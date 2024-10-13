#pragma once

#include <memory>
#include <logger.h>
#include "client/client.hpp"

class client;

class client_connection {
private:
    int _socket = -1;
    std::shared_ptr<client> _client = nullptr;
    std::chrono::steady_clock::time_point _connection_time;
    std::chrono::steady_clock::time_point _last_ping_timestamp;
    bool _handshake = false;

    std::shared_ptr<log4cxx::Logger> _client_logger;

public:
    explicit client_connection(int socket);

    [[nodiscard]] int get_socket() const;

    void set_client(const std::shared_ptr<client>& client);

    [[nodiscard]] std::shared_ptr<client> get_client() const;

    [[nodiscard]] bool is_logged_in() const;

    [[nodiscard]] bool is_handshake() const;

    void set_handshake(bool handshake);

    void update_ping_timestamp();

    bool is_timeout(int timeout);

    bool is_ping_timeout(int timeout);

    void disconnect();

    [[nodiscard]] bool is_alive() const;

    std::shared_ptr<log4cxx::Logger> get_logger();
};

