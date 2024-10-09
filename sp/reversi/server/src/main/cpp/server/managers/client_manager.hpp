#pragma once

#include <string>
#include <memory>
#include <list>
#include "../connection/client_info.hpp"

class client_manager {
private:
    std::shared_ptr<std::list<std::shared_ptr<client_info>>> _clients = std::make_shared<std::list<std::shared_ptr<client_info>>>();

public:

    std::shared_ptr<std::mutex> client_mutex = std::make_shared<std::mutex>();


    std::shared_ptr<client_info> add_client_connection(int client_socket, const std::string& client_address);


};

