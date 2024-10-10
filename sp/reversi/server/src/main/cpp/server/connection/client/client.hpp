#pragma once

#include <string>
#include <memory>
#include <chrono>
#include "../client_connection.hpp"
#include "../../../game/lobby/lobby.hpp"

class client_connection;

class client {
private:
    std::string _login;
    std::shared_ptr<client_connection> _connection;
    std::shared_ptr<lobby> _lobby;
    std::chrono::steady_clock::time_point _disconnection_timestamp;

public:
    void disconnected();
};

