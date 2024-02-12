#include <netinet/in.h>
#include <logger.h>
#include "server.hpp"
#include "../config/configuration.hpp"
#include "fmt/format.h"

static const auto LOGGER = log4cxx::Logger::getLogger("server");


[[noreturn]] void server::start() {
    this->server_socket  = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);

    LOGGER->debug(fmt::format("Starting server on the port: {}", this->port));

    int err = bind(this->server_socket, (struct sockaddr*)&address, sizeof(address));
    if (err != 0){
        LOGGER->error(fmt::format("Server socket binding ended with error code: {}", err));
        return;
    }

    err = listen(this->server_socket, 10);

    if (err != 0){
        LOGGER->error(fmt::format("Server socket listen ended with error code: {}", err));
        return;
    }

    while(true){
        dispatch_client();
    }


}

server::server() {
    this->connections = std::make_shared<std::set<std::shared_ptr<connection>>>();
    this->port = (int)configuration::get_file("config")->GetLongValue("Server", "port");
}

void server::dispatch_client() {
    sockaddr_in client_address{};
    socklen_t client_len = sizeof(client_address);
    int client_socket = accept(this->server_socket, (struct sockaddr*)&client_address, &client_len);

    auto conn = std::make_shared<connection>(client_socket);
    this->connections->insert(conn);
    conn->dispatch();

    auto test_payload = std::make_shared<payload>();
    test_payload->set_value("test", std::make_shared<integer>(666));
    auto test_header = std::make_shared<header>("POKR", type::POST, subtype::PING, status::OK);\
    auto test_msg = std::make_shared<message>(test_header, test_payload);
    while(conn->is_alive()){
        conn->send_message(test_msg);
        sleep(2);
    }
}
