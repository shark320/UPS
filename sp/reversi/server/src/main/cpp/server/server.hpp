#pragma once


#include <memory>
#include <thread>
#include <unordered_map>
#include "managers/client_manager.hpp"
#include "managers/lobby_manager.hpp"
#include "connection/client_info.hpp"
#include "SimpleIni.h"
#include "../game/lobby/lobby.hpp"
#include "connection/client/client.hpp"
#include "config/server_config.hpp"

class server {
public:
    server(const std::shared_ptr<server_config>& _server_config);

    ~server();

    void start();

private:
    std::shared_ptr<client_manager> _client_manager;
    std::shared_ptr<lobby_manager> _lobby_manager;
    std::shared_ptr<server_config> _server_config;
    std::shared_ptr<std::unordered_map<int, std::shared_ptr<std::thread>>> _client_threads = std::make_shared<std::unordered_map<int, std::shared_ptr<std::thread>>>();
    std::shared_ptr<std::thread> _client_timeouts_thread;
    int _socket;

    void check_start_preconditions();

    void create_socket();

    void bind_socket();

    void close_client_connection(std::shared_ptr<client_info> client_connection);

    void process_client_connection(std::shared_ptr<client_info> client_connection);

    void player_disconnected(std::shared_ptr<lobby> _lobby, std::shared_ptr<client> _client);

    void client_connection_timeout(std::shared_ptr<client> _client);

    void client_disconnected(std::shared_ptr<client> _client, std::shared_ptr<lobby> _lobby);

    void client_disconnected(std::shared_ptr<client> _client);

    void check_client_timeouts_thread();

    void start_check_client_timeouts_thread();

    void join_timeout_thread();

    [[noreturn]] void handle_incoming_connections();
};

