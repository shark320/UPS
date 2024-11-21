#include "client_manager.hpp"

std::shared_ptr<client_connection>
client_manager::add_client_connection(int client_socket, const std::string &client_address) {
    std::unique_lock<std::shared_mutex> unique_lock(*this->shared_mutex);
    auto _client_connection = std::make_shared<client_connection>(client_socket);
    (*this->_client_connections)[client_socket] = _client_connection;
    return _client_connection;
}

std::shared_ptr<client_conenctions_map_t> client_manager::get_client_connections() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    return this->_client_connections;
}

std::shared_ptr<clients_map_t> client_manager::get_clients() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    return this->_clients;
}

bool client_manager::is_login_taken(const std::string &login) {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    bool is_taken = false;
    is_taken = _clients->contains(login);
    return is_taken;
}

void client_manager::remove_client_connection(int client_socket) {
    std::unique_lock<std::shared_mutex> unique_lock(*this->shared_mutex);
    this->_client_connections->erase(client_socket);
}

std::shared_ptr<client> client_manager::get_client_by_login(const std::string &login) const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    auto client_it = this->_clients->find(login);
    if (client_it == this->_clients->end()) {
        return nullptr;
    }
    return client_it->second;
}

std::shared_ptr<client>
client_manager::login_client(const std::string &username, const std::shared_ptr<client_connection> &connection) {
    std::unique_lock<std::shared_mutex> unique_lock(*this->shared_mutex);
    auto new_client = std::make_shared<client>(username, connection);
    _clients->insert({username, new_client});
    return new_client;
}

std::shared_ptr<client_conenctions_map_t> client_manager::get_client_connections_unsafe() const {
    return this->_client_connections;
}

void client_manager::logout_client(const std::shared_ptr<client_connection> &connection) {
    const auto client = connection->get_client();
    if (client == nullptr){
        return;
    }
    connection->set_client(nullptr);
    client->disconnected();
}
