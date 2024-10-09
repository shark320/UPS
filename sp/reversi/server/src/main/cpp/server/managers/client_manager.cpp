#include "client_manager.hpp"

std::shared_ptr<client_info>
client_manager::add_client_connection(int client_socket, const std::string &client_address) {
    client_mutex->lock();
    auto _client_info = std::make_shared<client_info>(client_socket);
    this->_clients->push_back(_client_info);
    client_mutex->unlock();
    return _client_info;
}
