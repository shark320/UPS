#include "client.hpp"
#include "fmt/format.h"
#include "../../../utils/utils.hpp"

#include <utility>

void client::disconnected() {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    this->_connection = nullptr;
    this->_disconnection_timestamp = std::make_shared<std::chrono::steady_clock::time_point>(
            std::chrono::steady_clock::now());
}

bool client::is_connected() {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    return this->_connection != nullptr;
}

void client::connect(const std::shared_ptr<client_connection> &connection) {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    this->_connection = connection;
    this->_disconnection_timestamp = nullptr;
}

client::client(std::string username, const std::shared_ptr<client_connection> &connection) : _username(
        std::move(username)), _connection(connection) {

}

std::shared_ptr<client_connection> client::get_connection() {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    return this->_connection;
}

flow_state client::get_flow_state() const {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    return this->_flow_state;
}

void client::set_lobby(const std::shared_ptr<lobby> &lobby) {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    this->_lobby = lobby;
}

std::string client::get_username() const {
    return this->_username;
}

std::shared_ptr<lobby> client::get_lobby() const {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    return this->_lobby;
}

bool client::operator==(const client &other) const {
    return this->_username == other._username;
}

void client::clear_lobby() {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    this->_lobby = nullptr;
}

std::string client::to_string() const {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    std::string lobby_name = this->_lobby == nullptr ? "null" : fmt::format("'{}'", this->_lobby->get_name_unsafe());
    std::string disconnection_timestamp_str = this->_disconnection_timestamp == nullptr ? "null" : format_timestamp(this->_disconnection_timestamp);
    return fmt::format(
            "client(username='{}', flow_state={}, disconnection_timestamp={}, lobby={})",
            this->_username,
            flow_state_mapper::get_string(this->_flow_state),
            disconnection_timestamp_str,
            lobby_name
            );
}

void client::update_flow_state(flow_state new_state) {
    std::lock_guard<std::mutex> lock_guard(*this->_client_mutex);
    this->_flow_state = new_state;
}
