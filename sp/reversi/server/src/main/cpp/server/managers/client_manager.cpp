#include "client_manager.hpp"

std::shared_ptr<client_connection>
client_manager::add_client_connection(int client_socket, const std::string &client_address) {
    client_manager_mutex->lock();
    auto _client_connection = std::make_shared<client_connection>(client_socket);
    (*this->_client_connections)[client_socket] = _client_connection;
    client_manager_mutex->unlock();
    return _client_connection;
}

std::shared_ptr<client_conenctions_map_t> client_manager::get_client_connections() const {
    return this->_client_connections;
}

std::shared_ptr<clients_map_t> client_manager::get_clients() const {
    return this->_clients;
}

bool client_manager::is_login_taken(const std::string &login) {
    return this->_clients->contains(login);
}
