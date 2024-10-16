#pragma once

#include <memory>
#include "SimpleIni.h"

class server_config {
private:
    int port = -1;
    int client_queue_size = -1;
    int handshake_timeout = -1;
    int timeout_check_interval = -1;
    int login_timeout = -1;
    int ping_timeout = -1;
public:
    server_config(const std::shared_ptr<CSimpleIniA>& ini_config);

    server_config();

    [[nodiscard]] bool is_complete() const;

    [[nodiscard]] int get_port() const;

    [[nodiscard]] int get_client_queue_size() const;

    [[nodiscard]] int get_handshake_timeout() const;

    [[nodiscard]] int get_timeout_check_interval() const;

    [[nodiscard]] int get_login_timeout() const;

    [[nodiscard]] int get_ping_timeout() const;

    void init(const std::shared_ptr<CSimpleIniA>& ini_config);

    [[nodiscard]] std::string to_string() const;
};

