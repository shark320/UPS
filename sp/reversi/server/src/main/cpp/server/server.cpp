#include <asm-generic/socket.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <logger.h>
#include "server.hpp"
#include "fmt/format.h"

static auto LOGGER = log4cxx::Logger::getLogger("server");

server::server(const std::shared_ptr<server_config>& _server_config): _server_config(_server_config) {

}

void server::start() {
    check_start_preconditions();

    create_socket();
    bind_socket();

    if (listen(this->_socket, this->_server_config->get_client_queue_size()) != 0) {
        throw std::runtime_error("Error during socket listener creation.");
    }
    LOGGER->debug(fmt::format("Server is listening to port: {}. Clients connection queue size: {}.", this->_server_config->get_port(), this->_server_config->get_client_queue_size()));
    start_check_client_timeouts_thread();
}


void server::check_start_preconditions() {
    LOGGER->debug("Checking server start preconditions.");
    if(this->_server_config == nullptr || !this->_server_config->is_complete()){
        throw std::runtime_error("No server configuration is set.");
    }
}

void server::create_socket() {
    LOGGER->debug("Creating server socket.");
    this->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == -1) {
        throw std::runtime_error("Socket creation failed");
    }
    int reuse_address = 1;
    if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(int)) != 0) {
        throw std::runtime_error("Error during socket options setting.");
    }
}

void server::bind_socket() {
    LOGGER->debug("Binding server socket.");
    sockaddr_in addr_in{};
    auto port = this->_server_config->get_port();
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(port);
    addr_in.sin_addr.s_addr = INADDR_ANY;

    if (::bind(this->_socket, (sockaddr*)&addr_in, sizeof(addr_in)) != 0) {
        throw std::runtime_error("Socket address binding failed.");
    }
}

void server::start_check_client_timeouts_thread() {
    this->_client_timeouts_thread = std::make_shared<std::thread>(&server::check_client_timeouts_thread, this);
}

void server::join_timeout_thread() {
    if (this->_client_timeouts_thread && this->_client_timeouts_thread->joinable()) {
        this->_client_timeouts_thread->join();
    }
}

server::~server() {
    this->join_timeout_thread();
}

[[noreturn]] void server::handle_incoming_connections() {
    sockaddr_in incoming_socket{};
    socklen_t incoming_socket_length = sizeof(incoming_socket);

    while (true) {
        int incoming_connection = accept(this->_socket, (sockaddr*)(&incoming_socket), &incoming_socket_length);
        if (incoming_connection >= 0) {
            char client_addr[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &incoming_socket.sin_addr, client_addr, INET_ADDRSTRLEN) == NULL){
                LOGGER->error("An error occurred during address conversion.");
                close(incoming_connection);
                continue;
            }

            auto client_info = this->_client_manager->add_client_connection(incoming_connection, client_addr);
            (*this->_client_threads)[incoming_connection] = std::make_shared<std::thread>(&server::process_client_connection, this, client_info);
        }
    }
}

void server::process_client_connection(std::shared_ptr<client_info> client_connection) {

}


