#include "message_manager.hpp"
#include "../connection/consts/consts.hpp"
#include "fmt/format.h"

std::shared_ptr<message> message_manager::process(const std::shared_ptr<message> &request,
                                                  const std::shared_ptr<client_connection> &client_connection) {
    if (!check_identifier(request)) {
        return nullptr;
    }
    //Check preconditions: if handshake is performed
    if (!client_connection->is_handshake() && (request->get_header()->get_type() != type::POST ||
                                               request->get_header()->get_subtype() != subtype::HANDSHAKE)) {
        const std::string msg ="The client handshake was not performed.";
        client_connection->get_logger()->error(msg);
        return bad_request(request, msg);
    }
    //Check preconditions: if login is performed
    if (client_connection->get_client() == nullptr && (request->get_header()->get_type() != type::POST ||
                                                       request->get_header()->get_subtype() != subtype::LOGIN)){
        const std::string msg = "The client login was not performed.";
        client_connection->get_logger()->error(msg);
        return bad_request(request, msg);
    }
    switch (request->get_header()->get_type()) {
        case type::GET:
            return process_get(request, client_connection);
        case type::POST:
            return process_post(request, client_connection);
        default:
            return bad_request(request);
    }
}

std::shared_ptr<message>
message_manager::process_post(const std::shared_ptr<message> &request,
                              const std::shared_ptr<client_connection> &client_connection) {
    switch (request->get_header()->get_subtype()) {
        case subtype::HANDSHAKE:
            return process_handshake(request, client_connection);
        case subtype::LOGIN:
            return process_login(request, client_connection);
        case subtype::CREATE_GAME:
            process_create_new_game(request, client_connection);
        default:
            return bad_request(request);
    }
}

std::shared_ptr<message> message_manager::process_get(const std::shared_ptr<message> &request,
                                                      const std::shared_ptr<client_connection> &client_connection) {
    const auto request_subtype = request->get_header()->get_subtype();
    switch (request_subtype) {
        default:
            return bad_request(request);
    }
}

std::shared_ptr<message> message_manager::process_handshake(const std::shared_ptr<message> &request,
                                                            const std::shared_ptr<client_connection> &client_connection) {
    auto client_logger = client_connection->get_logger();
    client_logger->debug("Processing client handshake.");
    if (client_connection->is_handshake()) {
        client_logger->error("The client handshake is already done.");
        return bad_request(request, "The client handshake is already done.");
    }
    auto _response = std::make_shared<message>();
    auto _header = std::make_shared<header>(request->get_header());
    _header->set_length(0);
    _header->set_status(status::OK);
    _response->set_header(_header);

    client_connection->set_handshake(true);
    client_connection->update_ping_timestamp();

    return _response;
}

bool message_manager::check_identifier(const std::shared_ptr<message> &request) const {
    std::string identifier = request->get_header()->get_identifier();

    return identifier == this->_config->get_identifier();
}

void message_manager::set_client_manager(const std::shared_ptr<client_manager> &client_manager) {
    this->_client_manager = client_manager;
}

void message_manager::set_lobby_manager(const std::shared_ptr<lobby_manager> &lobby_manager) {
    this->_lobby_manager = lobby_manager;
}

message_manager::message_manager(const std::shared_ptr<connection_config> &connection_config) : _config(
        connection_config) {

}

std::shared_ptr<message> message_manager::process_login(const std::shared_ptr<message> &request,
                                                        const std::shared_ptr<client_connection> &client_connection) {
    auto client_logger = client_connection->get_logger();
    client_logger->debug("Processing login.");
    if (auto client = client_connection->get_client()) {
        client_logger->error("The client is already logged in.");
        return bad_request(request, "The client is already logged in.");
    }
    auto _response = std::make_shared<message>();
    auto response_header = std::make_shared<header>(request->get_header());
    auto request_payload = request->get_payload();
    auto response_payload = std::make_shared<payload>();
    auto login = request_payload->get_string("username");

    if (login == nullptr || login->empty()) {
        return bad_request(request);
    }

    auto _client = this->_client_manager->get_client_by_login(*login);

    if (_client != nullptr) {
        if (_client->is_connected()) {
            client_logger->debug(fmt::format("Client with the username '{}' is already connected!", *login));
            response_header->set_status(status::CONFLICT);
            response_payload->set_value("msg", std::make_shared<string>(
                    "Provided username is already in use. Choose another one, please."));


        } else {
            client_logger->debug(fmt::format("Found client with the username '{}' in cache.", *login));
            _client->connect(client_connection);
        }

    } else {
        client_logger->debug(fmt::format("New client with the name '{}' is logged in", *login));
        _client = this->_client_manager->login_client(*login, client_connection);
        client_connection->set_client(_client);
    }

    response_header->set_status(status::OK);
    response_payload->set_value("state",
                                std::make_shared<string>(flow_state_mapper::get_string(_client->get_flow_state())));

    return std::make_shared<message>(response_header, response_payload);
}

std::shared_ptr<message> message_manager::bad_request(const std::shared_ptr<message> &request) {
    return bad_request(request, "Bad request");
}

std::shared_ptr<message> message_manager::bad_request(const std::shared_ptr<message> &request, const std::string &msg) {
    auto _header = std::make_shared<header>(request->get_header());
    auto _payload = std::make_shared<payload>();
    _header->set_status(status::BAD_REQUEST);
    _payload->set_value("msg", std::make_shared<string>(msg));

    return std::make_shared<message>(_header, _payload);
}

std::shared_ptr<message> message_manager::process_create_new_game(const std::shared_ptr<message> &request,
                                                                  const std::shared_ptr<client_connection> &client_connection) {
    auto client_logger = client_connection->get_logger();
    if(!client_connection->is_logged_in()){
        return bad_request(request, "Cannot create a game: Client is no logged in!");
    }
    auto request_payload = request->get_payload();
    auto name_str_ptr = request_payload->get_string("name");
    if (name_str_ptr == nullptr || name_str_ptr->empty()){
        return bad_request(request, "Cannot create a game: Invalid game name!");
    }
    client_logger->debug(fmt::format("Performing game creation with name {}", *name_str_ptr));
    auto response_header = std::make_shared<header>(request->get_header());
    auto response_payload = std::make_shared<payload>();

    auto new_lobby = this->_lobby_manager->create_lobby(*name_str_ptr, client_connection->get_client());
    return std::shared_ptr<message>();
}

