#include "message_manager.hpp"
#include "../connection/consts/consts.hpp"
#include "fmt/format.h"

std::shared_ptr<message> message_manager::process(const std::shared_ptr<message> &request,
                                                  const std::shared_ptr<client_connection> &client_connection) {
    std::shared_ptr<message> response = nullptr;
    if (!check_identifier(request)) {
        return response;
    }
    switch (request->get_header()->get_type()) {
        case type::GET:
            //TODO: process GET actions
            break;
        case type::POST:
            response = process_post(request, client_connection);
            break;
        default:
            //TODO: error message
            response = nullptr;
            break;
    }
    return response;
}

std::shared_ptr<message>
message_manager::process_post(const std::shared_ptr<message> &request,
                              const std::shared_ptr<client_connection> &client_connection) {
    std::shared_ptr<message> response = nullptr;
    switch (request->get_header()->get_subtype()) {
        case subtype::HANDSHAKE:
            response = process_handshake(request, client_connection);
            break;
        case subtype::LOGIN:
            response = process_login(request, client_connection);
            break;
        default:
            response = bad_request(request);
            break;
    }
    return response;
}

std::shared_ptr<message> message_manager::process_handshake(const std::shared_ptr<message> &request,
                                                            const std::shared_ptr<client_connection> &client_connection) {
    auto client_logger = client_connection->get_logger();
    client_logger->debug("Processing client handshake.");
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
        return bad_request(request, "The client is already logged in");
    }
    auto _response = std::make_shared<message>();
    auto response_header = std::make_shared<header>(request->get_header());
    auto request_payload = request->get_payload();
    auto response_payload = std::make_shared<payload>();
    auto login = request_payload->get_string("username");
    if (login == nullptr || login->empty()) {
        return bad_request(request);
    }

    if (auto logged_client = this->_client_manager->get_client_by_login(*login)) {
        if (logged_client->is_connected()) {
            client_logger->debug(fmt::format("Client with the username '{}' is already connected!", *login));
            response_header->set_status(status::CONFLICT);
            response_payload->set_value("msg", std::make_shared<string>(
                    "Provided username is already in use. Choose another one, please."));


        } else {
            client_logger->debug(fmt::format("Found client with the username '{}' in cache.", *login));
            logged_client->connect(client_connection);
            //TODO: transition to the previous state in case of found client
        }

    } else {
        client_logger->debug(fmt::format("New client with the name '{}' is logged in", *login));
        auto client = this->_client_manager->login_client(*login, client_connection);
        client_connection->set_client(client);
    }


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
