#include "message_manager.hpp"
#include "../connection/consts/consts.hpp"

std::shared_ptr<message> message_manager::process(const std::shared_ptr<message>& request, const std::shared_ptr<client_connection>& client_connection) {
    std::shared_ptr<message> response = nullptr;
    if (!check_identifier(request)){
        //TODO: error message
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


    return std::shared_ptr<message>();
}

bool message_manager::check_identifier(const std::shared_ptr<message> &request) const{
    std::string identifier = request->get_header()->get_identifier();

    return identifier == this->_config->get_identifier();
}
