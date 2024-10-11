#include "message_manager.hpp"
#include "../connection/consts/consts.hpp"

std::shared_ptr<message> message_manager::process(const std::shared_ptr<message>& request, const std::shared_ptr<client_connection>& client_connection) {
    std::shared_ptr<message> response = nullptr;
    if (!check_identifier(request)){
        return response;
    }
    switch (request->get_header()->get_type()) {
        case type::GET:
            //TODO: process GET actions
            break;
        case type::POST:
            process_post(request, client_connection);
        default:
            //TODO: error message
            response = nullptr;
            break;
    }
    return response;
}

std::shared_ptr<message>
message_manager::process_post(const std::shared_ptr<message>& request, const std::shared_ptr<client_connection>& client_connection) {
    std::shared_ptr<message> response = nullptr;
    switch (request->get_header()->get_subtype()){
        case subtype::HANDSHAKE:
            process_handshake(request, client_connection);
            break;
        default:
            //TODO: error message
            response = nullptr;
            break;
    }
    return response;
}

std::shared_ptr<message> message_manager::process_handshake(const std::shared_ptr<message> &request,
                                                            const std::shared_ptr<client_connection> &client_connection) {
    auto _response = std::make_shared<message>();
    auto _header = std::make_shared<header>(request->get_header());
    _header->set_length(0);
    _header->set_status(status::OK);
    _response->set_header(_header);

    client_connection->set_handshake(true);
    client_connection->update_ping_timestamp();

    return _response;
}

bool message_manager::check_identifier(const std::shared_ptr<message> &request) const{
    std::string identifier = request->get_header()->get_identifier();

    return identifier == this->_config->get_identifier();
}

void message_manager::set_client_manager(const std::shared_ptr<client_manager> &client_manager) {
    this->_client_manager = client_manager;
}

void message_manager::set_lobby_manager(const std::shared_ptr<lobby_manager> &lobby_manager) {
    this->_lobby_manager = lobby_manager;
}

message_manager::message_manager(const std::shared_ptr<connection_config> &connection_config): _config(connection_config) {

}
