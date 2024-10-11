#pragma once

#include <memory>
#include "client_manager.hpp"
#include "lobby_manager.hpp"
#include "../connection/message/message.hpp"
#include "../config/server_config.hpp"
#include "../connection/connection_config/connection_config.hpp"

class message_manager {
private:
    std::shared_ptr<connection_config> _config;
    std::shared_ptr<client_manager> _client_manager = std::make_shared<client_manager>();
    std::shared_ptr<lobby_manager> _lobby_manager = std::make_shared<lobby_manager>();


    std::shared_ptr<message> process_handshake(const std::shared_ptr<message>& request, const std::shared_ptr<client_connection>& client_connection);

    std::shared_ptr<message> process_get(const std::shared_ptr<message>& request, const std::shared_ptr<client_connection>& client_connection);

    std::shared_ptr<message> process_post(const std::shared_ptr<message>& request, const std::shared_ptr<client_connection>& client_connection);

    bool check_identifier(const std::shared_ptr<message>& request) const;

//    std::shared_ptr<message> process_unknown_type(std::shared_ptr<message> request, std::shared_ptr<client_connection> client_connection);
//
//    std::shared_ptr<message>
public:
    std::shared_ptr<message>
    process(const std::shared_ptr<message>& request, const std::shared_ptr<client_connection>& client_connection);

    explicit message_manager(const std::shared_ptr<connection_config>& connection_config);

    void set_client_manager(const std::shared_ptr<client_manager>& client_manager);

    void set_lobby_manager(const std::shared_ptr<lobby_manager>& lobby_manager);

};

