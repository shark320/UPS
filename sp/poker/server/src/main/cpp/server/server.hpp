#pragma once

#include <set>
#include <memory>
#include "connection/connection.hpp"

class server {
private:
    std::shared_ptr<std::set<std::shared_ptr<connection>>> connections;

    int port;

    int server_socket;

    void dispatch_client();

public:

    server();

    [[noreturn]] void start();
};

