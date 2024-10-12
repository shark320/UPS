#pragma once

#include <string>
#include <memory>
#include <list>
#include "../connection/client_connection.hpp"

using client_conenctions_map_t = std::unordered_map<int, std::shared_ptr<client_connection>>;

using clients_map_t = std::unordered_map<std::string, std::shared_ptr<client>>;

class client_manager {
private:
    std::shared_ptr<client_conenctions_map_t> _client_connections = std::make_shared<client_conenctions_map_t>();
    std::shared_ptr<clients_map_t> _clients = std::make_shared<clients_map_t>();
public:

    std::shared_ptr<std::mutex> client_manager_mutex = std::make_shared<std::mutex>();

    std::shared_ptr<client_connection> add_client_connection(int client_socket, const std::string &client_address);

    [[nodiscard]] std::shared_ptr<client_conenctions_map_t> get_client_connections() const;

    [[nodiscard]] std::shared_ptr<clients_map_t> get_clients() const;

    [[nodiscard]] bool is_login_taken (const std::string& login);

};

