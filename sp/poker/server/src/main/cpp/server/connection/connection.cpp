#include <sys/socket.h>
#include <logger.h>
#include <functional>

#include "connection.hpp"
#include "fmt/format.h"
#include "../../utils/global.hpp"
#include "consts/consts.hpp"

static const auto LOGGER = log4cxx::Logger::getLogger("connection");

static const int BUFFER_SIZE = 5096;

connection::connection(int socket) : socket(socket) {
    this->received_messages = std::make_shared<blocking_queue<std::shared_ptr<message>>>();
    this->send_queue = std::make_shared<blocking_queue<std::shared_ptr<message>>>();
}

void connection::dispatch() {
    this->is_alive_ = true;
    this->receiving_thread = std::make_shared<std::jthread>(&connection::handle_receive, this);
    this->sending_thread = std::make_shared<std::jthread>(&connection::handle_send, this);
}

void connection::handle_receive() {
    LOGGER->debug(fmt::format("Starting receiving handler thread on the socket {}", this->socket));
    while (true) {
        std::shared_ptr<message> msg = receive_message();

        if (msg == nullptr){
            //TODO: handle client closing
            LOGGER->debug(fmt::format("Client socket {} is closed.", this->socket));
            this->is_alive_ = false;
            terminate_threads();
            break;
        }

        LOGGER->trace(fmt::format("Received message '{}' from the socket {}", msg->to_string(), this->socket));
        this->received_messages->push(msg);
    }
}

std::shared_ptr<message> connection::receive_message() {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(this->socket, buffer, constants::MSG_HEADER_LENGTH, 0);
    if (bytes_received <= 0) {
        return nullptr;
    }
    std::string header_str = std::string(buffer, bytes_received);
    std::shared_ptr<header> header_ = header::extract(header_str);
    bytes_received = recv(this->socket, buffer, header_->get_length(), 0);
    if (bytes_received <= 0) {
        return nullptr;
    }
    std::shared_ptr<payload> payload_ = payload::parse(std::string(buffer, bytes_received));

    return std::make_shared<message>(header_, payload_);
}


void connection::handle_send() {
    LOGGER->debug(fmt::format("Starting sending handler thread on the socket {}", this->socket));
    while (true) {
        // Pop a message from the send queue and send it
        std::shared_ptr<message> msg = send_queue->pop();
        LOGGER->debug(fmt::format("Sending message '{}' to the socket {}", msg->to_string(), this->socket));
        std::string msg_str = msg->construct();
        ssize_t err = send(this->socket, msg_str.c_str(), msg_str.size(), 0);
        if (err < 0) {
            this->is_alive_ = false;
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

const std::shared_ptr<blocking_queue<std::shared_ptr<message>>> &connection::get_received_messages() const {
    return received_messages;
}

const std::shared_ptr<blocking_queue<std::shared_ptr<message>>> &connection::get_send_queue() const {
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

void connection::send_message(std::shared_ptr<message> msg) {
    this->send_queue->push(msg);
}

