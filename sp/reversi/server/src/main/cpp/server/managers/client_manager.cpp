#include "client_manager.hpp"

std::shared_ptr<client_connection>
client_manager::add_client_connection(int client_socket, const std::string &client_address) {
    client_manager_mutex->lock();
    auto _client_info = std::make_shared<client_connection>(client_socket);
    this->_clients->push_back(_client_info);
    client_manager_mutex->unlock();
    return _client_info;
}

std::shared_ptr<std::list<std::shared_ptr<client_connection>>> client_manager::get_clients() const {
    return this->_clients;
}
