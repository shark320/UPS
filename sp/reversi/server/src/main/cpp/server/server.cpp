#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server.hpp"
#include "fmt/format.h"
#include "connection/consts/consts.hpp"
#include "connection/message/header/header.hpp"
#include "../utils/utils.hpp"
#include "connection/message/payload/payload.hpp"
#include "connection/message/message.hpp"

static auto LOGGER = log4cxx::Logger::getLogger("server");

server::server(const std::shared_ptr<server_config> &_server_config) : _server_config(_server_config) {

}

void server::start() {
    check_start_preconditions();

    create_socket();
    bind_socket();

    if (listen(this->_socket, this->_server_config->get_client_queue_size()) != 0) {
        throw std::runtime_error("Error during _socket listener creation.");
    }
    LOGGER->debug(fmt::format("Server is listening to port: {}. Clients connection queue size: {}.",
                              this->_server_config->get_port(), this->_server_config->get_client_queue_size()));
    start_check_client_timeouts_thread();
    handle_incoming_connections();
}


void server::check_start_preconditions() {
    LOGGER->debug("Checking server start preconditions.");
    if (this->_server_config == nullptr || !this->_server_config->is_complete()) {
        throw std::runtime_error("No server configuration is set.");
    }
}

void server::create_socket() {
    LOGGER->debug("Creating server _socket.");
    this->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == -1) {
        throw std::runtime_error("Socket creation failed");
    }
    int reuse_address = 1;
    if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(int)) != 0) {
        throw std::runtime_error("Error during _socket options setting.");
    }
}

void server::bind_socket() {
    LOGGER->debug("Binding server _socket.");
    sockaddr_in addr_in{};
    auto port = this->_server_config->get_port();
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(port);
    addr_in.sin_addr.s_addr = INADDR_ANY;

    if (::bind(this->_socket, (sockaddr *) &addr_in, sizeof(addr_in)) != 0) {
        throw std::runtime_error("Socket address binding failed.");
    }
}

void server::start_check_client_timeouts_thread() {
    LOGGER->debug("Starting timeouts checker thread.");
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
        int incoming_connection = accept(this->_socket, (sockaddr *) (&incoming_socket), &incoming_socket_length);
        if (incoming_connection >= 0) {
            char client_addr[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &incoming_socket.sin_addr, client_addr, INET_ADDRSTRLEN) == NULL) {
                LOGGER->error("An error occurred during address conversion.");
                close(incoming_connection);
                continue;
            }

            auto client_info = this->_client_manager->add_client_connection(incoming_connection, client_addr);
            (*this->_client_threads)[incoming_connection] = std::make_shared<std::thread>(
                    &server::process_client_connection, this, client_info);
        }
    }
}

void server::process_client_connection(const std::shared_ptr<client_connection> &client_connection) {
    int socket = client_connection->get_socket();
    auto client_logger = log4cxx::Logger::getLogger(fmt::format("client({})", socket));

    try {
        client_logger->debug(fmt::format("New client connection established!"));
        //TODO: set to 0
        size_t received = 0;
        size_t tmp_received = 0;
        do {
            std::vector<char> header_data(constants::MSG_HEADER_LENGTH + 1, 0);
            tmp_received = recv(socket, header_data.data(), constants::MSG_HEADER_LENGTH, 0);

            //Connection closed
            if (tmp_received == 0) {
                //TODO: refactor message
                client_logger->debug("Connection closed.");
                close_client_connection(client_connection);
                break;
            }

            //Connection error
            if (tmp_received == -1) {
                //TODO: refactor message
                client_logger->debug("Error receiving data: " + std::string(strerror(errno)));
                close_client_connection(client_connection);
                break;
            }
            auto header_str = std::string(header_data.data());

            //Check if header is not empty
            if (is_whitespaces_only(header_str)) {
                client_logger->debug(fmt::format("Received message contains only whitespaces. Skipping.", socket));
                continue;
            }
            auto _header = header::extract(header_str);

            //Check header values
            if (!check_header(_header)) {
                client_logger->error(fmt::format("Invalid header: {}", _header->to_string()));
                close_client_connection(client_connection);
                break;
            }

            std::shared_ptr<payload> _payload = receive_payload(socket, received, _header->get_length(), client_logger);
            auto _message = std::make_shared<message>(_header, _payload);
            client_logger->debug(fmt::format("Received message: {}", _message->to_string()));



        } while (received > 0);

        detach_client_thread(client_connection);

    } catch (const std::exception &e) {
        client_logger->error(fmt::format("An error occurred during client connection processing: {}", e.what()));
        this->close_client_connection(client_connection);
    }
}

void server::close_client_connection(const std::shared_ptr<client_connection>& client_connection) {
    if  (!client_connection->is_alive()){
        return;
    }
    int socket = client_connection->get_socket();
    LOGGER->debug(fmt::format("Closing client connection for socket: {}", socket));
    shutdown(socket, SHUT_RDWR);
    close(socket);
    auto _client_thread = (*this->_client_threads)[socket];
    client_connection->disconnect();
    client_disconnected(client_connection->get_client());
}

void server::check_client_timeouts_thread() {
    auto logger = log4cxx::Logger::getLogger("timeout-checker");
    logger->debug("Timeouts checker thread started.");
    while (true) {
        std::this_thread::sleep_for(std::chrono::microseconds(this->_server_config->get_timeout_check_interval()));
        this->_client_manager->client_manager_mutex->lock();

        auto _clients = this->_client_manager->get_clients();
        for (auto _client_connection_it = _clients->begin(); _client_connection_it != _clients->end();) {
            bool timeout = false;
            auto _client_connection = *_client_connection_it;
            if (_client_connection == nullptr){
                continue;
            }
            if (!_client_connection->is_handshake() &&
                _client_connection->is_timeout(this->_server_config->get_handshake_timeout())){
                logger->debug(fmt::format("Client on socket {}: Handshake timeout reached.", _client_connection->get_socket()));
                timeout = true;
            } else if (!_client_connection->is_logged_in() && _client_connection->is_timeout(this->_server_config->get_login_timeout())){
                logger->debug(fmt::format("Client on socket {}: Login timeout reached.", _client_connection->get_socket()));
                timeout = true;
            } else if (_client_connection->is_handshake() && _client_connection->is_ping_timeout(this->_server_config->get_ping_timeout())) {
                logger->debug(
                        fmt::format("Client on socket {}: Ping timeout reached.", _client_connection->get_socket()));
                timeout = true;
            }
            if (timeout){
                close_client_connection(_client_connection);
                _client_connection_it = _clients->erase(_client_connection_it);
            } else{
                ++_client_connection_it;
            }
        }
        this->_client_manager->client_manager_mutex->unlock();
    }
}

bool server::check_header(std::shared_ptr<header> _header) {
    return _header->check_values() && _header->get_identifier() == constants::IDENTIFIER;
}

void server::detach_client_thread(const std::shared_ptr<client_connection> &client_connection) {
    int socket = client_connection->get_socket();
    this->_client_manager->client_manager_mutex->lock();
    auto client_thread = (*this->_client_threads)[socket];
    if (client_thread != nullptr) {
        if (client_thread->joinable()) {
            client_thread->detach();
        }
        this->_client_threads->erase(socket);
    }
    this->_client_manager->client_manager_mutex->unlock();
}

std::shared_ptr<payload> server::receive_payload(int socket, size_t &received, size_t payload_length,
                                                 const std::shared_ptr<log4cxx::Logger> &client_logger) {
    std::shared_ptr<payload> _payload = std::make_shared<payload>();
    size_t tmp_received = 0;
    if (payload_length != 0) {
        std::vector<char> payload_data(payload_length + 1, 0);
        received = 0;
        do {
            //loading packages
            tmp_received = recv(socket, payload_data.data() + received, payload_length - received, 0);
            client_logger->debug(fmt::format("Received {} bytes.", tmp_received));
            client_logger->debug(std::to_string(tmp_received));
            received += tmp_received;
        } while (received < payload_length && tmp_received > 0);
        auto payload_str = std::string(payload_data.data());
        _payload = payload::parse(payload_str);
        client_logger->debug(fmt::format("Received payload: {}", _payload->to_string()));
    } else {
        client_logger->debug(fmt::format("Received payload is empty."));
    }
    return _payload;
}

void server::client_timeout(std::shared_ptr<client> _client) {

}

void server::client_disconnected(std::shared_ptr<client> _client) {
    if (_client == nullptr){
        return;
    }
    //TODO: update game lobby
    _client->disconnected();
}


