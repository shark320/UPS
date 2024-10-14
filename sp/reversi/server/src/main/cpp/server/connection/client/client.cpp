#include "client.hpp"

#include <utility>

void client::disconnected() {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    this->_connection = nullptr;
    this->_disconnection_timestamp = std::chrono::steady_clock::now();
}

bool client::is_connected() {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    return this->_connection != nullptr;
}

void client::connect(const std::shared_ptr<client_connection>& connection) {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    this->_connection = connection;
}

client::client(std::string username, const std::shared_ptr<client_connection> &connection): _username(std::move(username)), _connection(connection) {

}

std::shared_ptr<client_connection> client::get_connection() {
    return this->_connection;
}

flow_state client::get_flow_state() const {
    return this->_flow_state;
}

void client::set_lobby(std::shared_ptr<lobby> lobby) {
    this->_lobby = lobby;
}
