#pragma once

#include <string>
#include <memory>
#include "../connection/client_info.hpp"

class client_manager {
public:
    std::shared_ptr<client_info> add_client_connection(int client_socket, const std::string& client_address);
};

