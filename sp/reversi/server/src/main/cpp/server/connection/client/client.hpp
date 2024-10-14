#pragma once

#include <string>
#include <memory>
#include <chrono>
#include "../client_connection.hpp"
#include "../../../game/lobby/lobby.hpp"
#include "enums/flow_state.hpp"

class client_connection;

class client {
private:
    std::string _username;
    std::shared_ptr<client_connection> _connection;
    std::shared_ptr<lobby> _lobby;
    std::chrono::steady_clock::time_point _disconnection_timestamp;
    flow_state _flow_state = flow_state::MENU;

    std::shared_ptr<std::mutex> _client_mutex = std::make_shared<std::mutex>();
public:
    client(std::string  login, const std::shared_ptr<client_connection>& connection);

    std::shared_ptr<client_connection> get_connection();

    void disconnected();

    bool is_connected();

    void connect(const std::shared_ptr<client_connection>& connection);

    [[nodiscard]] flow_state get_flow_state() const;
};

