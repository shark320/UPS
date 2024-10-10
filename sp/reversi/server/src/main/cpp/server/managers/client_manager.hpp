#pragma once

#include <string>
#include <memory>
#include <list>
#include "../connection/client_connection.hpp"

class client_manager {
private:
    std::shared_ptr<std::list<std::shared_ptr<client_connection>>> _clients = std::make_shared<std::list<std::shared_ptr<client_connection>>>();

public:

    std::shared_ptr<std::mutex> client_manager_mutex = std::make_shared<std::mutex>();

    std::shared_ptr<client_connection> add_client_connection(int client_socket, const std::string& client_address);

    [[nodiscard]] std::shared_ptr<std::list<std::shared_ptr<client_connection>>> get_clients() const;
};

