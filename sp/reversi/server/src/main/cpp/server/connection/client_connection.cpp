#include <chrono>
#include "client_connection.hpp"
#include "fmt/format.h"

int client_connection::get_socket() const {
    return this->_socket;
}

client_connection::client_connection(int socket) {
    this->_socket = socket;
    this->_connection_time = std::chrono::steady_clock::now();
    this->_client_logger = log4cxx::Logger::getLogger(fmt::format("client({})", socket));
}

bool client_connection::is_timeout(int timeout) {
    auto _now = std::chrono::steady_clock::now();
    auto _elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(_now - this->_connection_time).count();
    return _elapsed_time > timeout;
}

void client_connection::set_client(const std::shared_ptr<client>& client) {
    this->_client = client;
}

std::shared_ptr<client> client_connection::get_client() const {
    return this->_client;
}

bool client_connection::is_logged_in() const {
    return this->_client != nullptr;
}

bool client_connection::is_handshake() const {
    return this->_handshake;
}

void client_connection::set_handshake(bool handshake) {
    this->_handshake = handshake;
}

void client_connection::update_ping_timestamp() {
    this->_last_ping_timestamp = std::chrono::steady_clock::now();
}

bool client_connection::is_ping_timeout(int timeout) {
    auto _now = std::chrono::steady_clock::now();
    auto _elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(_now - this->_last_ping_timestamp).count();
    return _elapsed_time > timeout;
}

void client_connection::disconnect() {
    this->_socket = -1;
}

bool client_connection::is_alive() const {
    return this->_socket != -1;
}

std::shared_ptr<log4cxx::Logger> client_connection::get_logger() {
    return this->_client_logger;
}
