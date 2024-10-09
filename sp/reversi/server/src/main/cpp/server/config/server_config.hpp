#pragma once

#include <memory>
#include "SimpleIni.h"

class server_config {
private:
    int port = -1;
    int client_queue_size = -1;
    int handshake_timeout = -1;
public:
    server_config(const std::shared_ptr<CSimpleIniA>& ini_config);

    server_config();

    [[nodiscard]] bool is_complete() const;

    [[nodiscard]] int get_port() const;

    [[nodiscard]] int get_client_queue_size() const;

    [[nodiscard]] int get_handshake_timeout() const;

    void init(const std::shared_ptr<CSimpleIniA>& ini_config);

    std::string to_string() const;
};

