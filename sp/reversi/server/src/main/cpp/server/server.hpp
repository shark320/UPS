#pragma once

#include <logger.h>
#include <memory>
#include <thread>
#include <unordered_map>
#include "managers/client_manager.hpp"
#include "managers/lobby_manager.hpp"
#include "connection/client_connection.hpp"
#include "SimpleIni.h"
#include "../game/lobby/lobby.hpp"
#include "connection/client/client.hpp"
#include "config/server_config.hpp"
#include "connection/message/header/header.hpp"
#include "connection/message/payload/payload.hpp"
#include "managers/message_manager.hpp"

class server {
public:
    explicit server(const std::shared_ptr<server_config>& _server_config, const std::shared_ptr<message_manager>& _message_manager);

    ~server();

    void start();

private:
    std::shared_ptr<client_manager> _client_manager = std::make_shared<client_manager>();
    std::shared_ptr<lobby_manager> _lobby_manager = std::make_shared<lobby_manager>();
    std::shared_ptr<server_config> _server_config;
    std::shared_ptr<std::unordered_map<int, std::shared_ptr<std::thread>>> _client_threads = std::make_shared<std::unordered_map<int, std::shared_ptr<std::thread>>>();
    std::shared_ptr<std::thread> _client_timeouts_thread;
    std::shared_ptr<message_manager> _message_manager;
    int _socket;

    void check_start_preconditions();

    void create_socket();

    void bind_socket();

    void close_client_connection(const std::shared_ptr<client_connection>& client_connection);

    void process_client_connection(const std::shared_ptr<client_connection>& client_connection);

    void player_disconnected(std::shared_ptr<lobby> _lobby, std::shared_ptr<client> _client);

    void client_timeout(const std::shared_ptr<client>& _client);

    void client_disconnected(std::shared_ptr<client> _client, std::shared_ptr<lobby> _lobby);

    void client_disconnected(const std::shared_ptr<client>& _client);

    void check_client_timeouts_thread();

    void start_check_client_timeouts_thread();

    void join_timeout_thread();

    bool check_header(std::shared_ptr<header> _header);

    void detach_client_thread(const std::shared_ptr<client_connection> &client_connection);

    std::shared_ptr<payload> receive_payload(int socket, ssize_t &received, size_t payload_length, const std::shared_ptr<log4cxx::Logger>& client_logger);

    [[noreturn]] void handle_incoming_connections();
};

