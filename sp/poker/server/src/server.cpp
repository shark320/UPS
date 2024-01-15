#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include "base/base.hpp"
#include "connection/payload/payload.hpp"
#include "connection/consts/consts.hpp"
#include "connection/status.hpp"
#include "connection/enums/type.hpp"
#include "connection/enums/status.hpp"

#define PORT 10000

/*void testPayload(){
    auto payload = std::make_shared<Payload>();
    auto testString1 = std::make_shared<std::string>("Hello, dynamically allocated string!");
    auto testString2 = std::make_shared<std::string>("Hello, dynamically allocated string 2!");
    payload->setData("test1", testString1);
    payload->setData("test2", testString2);

    std::shared_ptr<void> value1 = payload->get_data("test1");
    std::shared_ptr<void> value2 = payload->get_data("test2");
    std::shared_ptr<void> value3 = payload->get_data("test3");

    if (value1) {
        std::shared_ptr<std::string> strValue1 = std::static_pointer_cast<std::string>(value1);
        std::cout << "Value for test1: " << *strValue1 << std::endl;
    } else {
        std::cout << "Value for test1 is null." << std::endl;
    }

    if (value2) {
        std::shared_ptr<std::string> strValue2 = std::static_pointer_cast<std::string>(value2);
        std::cout << "Value for test2: " << *strValue2 << std::endl;
    } else {
        std::cout << "Value for test2 is null." << std::endl;
    }

    if (value3) {
        std::shared_ptr<std::string> strValue3 = std::static_pointer_cast<std::string>(value3);
        std::cout << "Value for test3: " << *strValue3 << std::endl;
    } else {
        std::cout << "Value for test3 is null." << std::endl;
    }
}*/

/*void close_connection(int client_sock, std::thread::id thread_id) {
    std::cout << "[Thread:" << thread_id << "] Closing connection." << std::endl;
    close(client_sock);
}*/

/*
void serveConnection(int client_socket) {
    char buffer[1024]; // Buffer to store incoming data

    while (true) {
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer

        ssize_t bytesRead = recv(client_socket, buffer, sizeof(buffer), 0);
        std::cout << "Received bytes: " << bytesRead << std::endl;
        if (bytesRead <= 0) {
            // Handle errors or end of connection
            break;
        }
            std::string receivedMessage(buffer, bytesRead);
            std::cout << "Received: " << receivedMessage << std::endl;

    }

    close_connection(client_socket, std::this_thread::get_id());
}
*/

int main(){
    /*int server_sock;
    int client_sock;
    int return_value;
    char cbuf;
    sockaddr_in local_addr{};
    sockaddr_in remote_addr;
    socklen_t remote_addr_len;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sock <= 0) {
        std::cout << "Socket ERR" << std::endl;
        return -1;
    }

    std::memset(&local_addr, 0, sizeof(sockaddr_in));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    int param = 1;
    return_value = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &param, sizeof(int));

    if (return_value == -1)
        std::cout << "setsockopt ERR" << std::endl;

    return_value = bind(server_sock, reinterpret_cast<sockaddr*>(&local_addr), sizeof(sockaddr_in));

    if (return_value == 0)
        std::cout << "Bind OK" << std::endl;
    else {
        std::cout << "Bind ERR" << std::endl;
        return -1;
    }

    return_value = listen(server_sock, 5);
    if (return_value == 0)
        std::cout << "Listen OK" << std::endl;
    else {
        std::cout << "Listen ERR" << std::endl;
        return -1;
    }

    while (true) {
        client_sock = accept(server_sock, reinterpret_cast<sockaddr*>(&remote_addr), &remote_addr_len);

        if (client_sock > 0) {
            std::cout << "New connection!" << std::endl;
            std::thread(serveConnection, client_sock).detach();
        } else {
            std::cout << "Brutal Fatal ERROR" << std::endl;
            return -1;
        }
    }*/

    std:string test_payload_str = "str=\"string\";int=12;int_arr=[1,2,3,4]";
    char buffer[constants::MSG_MAX_LENGTH];
    std::sprintf(buffer, "%s%04d%1d%02d%03d%s", "POKR", static_cast<int>(test_payload_str.length()), static_cast<int>(type::_enum::GET), static_cast<int>(subtype::_enum::PING), static_cast<int>(status::_enum::OK), test_payload_str.c_str());
    std::string test_request(buffer);

    auto test_payload = payload::extract(test_request);

    std::cout << test_payload->to_string() << std::endl;

    return 0;
}