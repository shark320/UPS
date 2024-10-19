#pragma once

#include <string>
#include <memory>
#include <chrono>
#include "../client_connection.hpp"
#include "../../../game/lobby/lobby.hpp"
#include "enums/flow_state.hpp"

class client_connection;

class lobby;

class client {
private:
    std::string _username;
    std::shared_ptr<client_connection> _connection;
    std::shared_ptr<lobby> _lobby;
    std::shared_ptr<std::chrono::steady_clock::time_point> _disconnection_timestamp;
    flow_state _flow_state = flow_state::MENU;

    std::shared_ptr<std::shared_mutex> shared_mutex = std::make_shared<std::shared_mutex>();
public:
    client(std::string login, const std::shared_ptr<client_connection>& connection);

    std::shared_ptr<client_connection> get_connection();

    void update_flow_state(flow_state new_state);

    void disconnected();

    bool is_connected();

    void connect(const std::shared_ptr<client_connection>& connection);

    [[nodiscard]] flow_state get_flow_state() const;

    void set_lobby(const std::shared_ptr<lobby>& lobby);

    void clear_lobby();

    [[nodiscard]] std::string get_username() const;

    [[nodiscard]] std::shared_ptr<lobby> get_lobby() const;

    bool operator==(const client &other) const;

    [[nodiscard]] bool is_in_state(std::initializer_list<flow_state> states) const;

    [[nodiscard]] std::string to_string() const;
};

