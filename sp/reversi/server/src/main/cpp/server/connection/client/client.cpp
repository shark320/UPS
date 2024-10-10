#include "client.hpp"

void client::disconnected() {
    this->_connection = nullptr;
    this->_disconnection_timestamp = std::chrono::steady_clock::now();
}
