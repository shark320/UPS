#include <sys/socket.h>
#include <logger.h>
#include <functional>

#include "connection.hpp"
#include "fmt/format.h"
#include "../../utils/global.hpp"

static const auto LOGGER = log4cxx::Logger::getLogger("connection");

static const int BUFFER_SIZE = 5096;

connection::connection(int socket) : socket(socket) {
    this->received_messages = std::make_shared<blocking_queue<std::string>>();
    this->send_queue = std::make_shared<blocking_queue<std::string>>();
}

void connection::dispatch() {
    this->is_alive_ = true;
    this->receiving_thread = std::make_shared<std::jthread>(&connection::handle_receive, this);
    this->sending_thread = std::make_shared<std::jthread>(&connection::handle_send, this);
}

void connection::handle_receive() {
    LOGGER->debug(fmt::format("Starting receiving handler thread on the socket {}", this->socket));
    char buffer[BUFFER_SIZE];
    while (true) {
        // Receive data from the client
        ssize_t bytes_received = recv(this->socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            //TODO: handle client closing
            LOGGER->debug(fmt::format("Client socket {} is closed.", this->socket));
            this->is_alive_ = false;
            terminate_threads();
            break;
        }
        std::string msg(buffer, bytes_received);

        LOGGER->trace(fmt::format("Received message '{}' from the socket {}", msg, this->socket));
        this->received_messages->push(msg);
    }
}

void connection::handle_send() {
    LOGGER->debug(fmt::format("Starting sending handler thread on the socket {}", this->socket));
    while (true) {
        // Pop a message from the send queue and send it
        std::string msg = send_queue->pop();
        LOGGER->debug(fmt::format("Sending message '{}' to the socket {}", msg, this->socket));
        ssize_t err = send(this->socket, msg.c_str(), msg.size(), 0);
        if (err < 0) {
            LOGGER->debug(fmt::format("Error during sending message to the socket {}", this->socket));
            break;
        }
    }
}

int connection::get_socket() const {
    return socket;
}

bool connection::is_alive() const {
    return is_alive_;
}

const std::shared_ptr<blocking_queue<std::string>> &connection::get_received_messages() const {
    return received_messages;
}

const std::shared_ptr<blocking_queue<std::string>> &connection::get_send_queue() const {
    return send_queue;
}

const std::shared_ptr<std::jthread> &connection::get_receiving_thread() const {
    return receiving_thread;
}

const std::shared_ptr<std::jthread> &connection::get_sending_thread() const {
    return sending_thread;
}

void connection::terminate_receiving_thread() {
    LOGGER->debug(fmt::format("Terminating receiving handler thread on the socket {}.", this->socket));
    this->receiving_thread->request_stop();
}

void connection::terminate_sending_thread() {
    LOGGER->debug(fmt::format("Terminating sending handler thread on the socket {}.", this->socket));
    this->receiving_thread->request_stop();
}

void connection::terminate_threads() {
    terminate_receiving_thread();
    terminate_sending_thread();
}
