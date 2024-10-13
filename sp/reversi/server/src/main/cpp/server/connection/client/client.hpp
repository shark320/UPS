#pragma once

#include <string>
#include <memory>
#include <chrono>
#include "../client_connection.hpp"
#include "../../../game/lobby/lobby.hpp"

class client_connection;

class client {
private:
    std::string _username;
    std::shared_ptr<client_connection> _connection;
    std::shared_ptr<lobby> _lobby;
    std::chrono::steady_clock::time_point _disconnection_timestamp;

    std::shared_ptr<std::mutex> _client_mutex = std::make_shared<std::mutex>();
public:
    client(std::string  login, const std::shared_ptr<client_connection>& connection);

    std::shared_ptr<client_connection> get_connection();

    void disconnected();

    bool is_connected();

    void connect(const std::shared_ptr<client_connection>& connection);
};

