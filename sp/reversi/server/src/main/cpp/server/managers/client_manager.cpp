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
    bool is_taken = false;
    client_manager_mutex->lock();
    is_taken = _clients->contains(login);
    client_manager_mutex->unlock();
    return is_taken;
}

void client_manager::remove_client_connection(int client_socket) {
    client_manager_mutex->lock();
    this->_client_connections->erase(client_socket);
    client_manager_mutex->unlock();
}

std::shared_ptr<client> client_manager::get_client_by_login(const std::string &login) const {
    client_manager_mutex->lock();
    auto _client = (*this->_clients)[login];
    client_manager_mutex->unlock();
    return _client;
}

std::shared_ptr<client>
client_manager::login_client(const std::string username, const std::shared_ptr<client_connection> conenction) {
    std::lock_guard<std::mutex> lock_guard(*this->client_manager_mutex);
    auto new_client = std::make_shared<client>(username, conenction);
    _clients->insert({username, new_client});
    return new_client;
}
