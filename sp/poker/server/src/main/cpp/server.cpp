#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include <fmt/core.h>
#include "base/base.hpp"
#include "connection/message/payload/payload.hpp"
#include "connection/consts/consts.hpp"
#include "connection/message/enums/type.hpp"
#include "connection/message/enums/status.hpp"
#include "connection/message/header/header.hpp"
#include "config/configuration.hpp"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/xml/domconfigurator.h>


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

static auto LOGGER = log4cxx::Logger::getLogger("MyApp");

void test_payload_construct(){
    auto test_payload = std::make_shared<payload>();
    auto test_int_vector = std::make_shared<vector>();
    auto test_str_vector = std::make_shared<vector>();

    test_int_vector->push_back(std::make_shared<integer>(1));
    test_int_vector->push_back(std::make_shared<integer>(2));
    test_int_vector->push_back(std::make_shared<integer>(3));

    test_str_vector->push_back(std::make_shared<string>("str1"));
    test_str_vector->push_back(std::make_shared<string>("str2"));
    test_str_vector->push_back(std::make_shared<string>("str3"));

    test_payload->set_value("int_arr", test_int_vector);
    test_payload->set_value("str_arr", test_str_vector);
    test_payload->set_value("int", std::make_shared<integer>(111));
    test_payload->set_value("str", std::make_shared<string>("test_str"));
    test_payload->set_value("bool", std::make_shared<boolean>(true));
    test_payload->set_value("test_null", nullptr);

    printf("Test constructed payload: \t%s\n", test_payload->construct().c_str());
}

void test_payload_parse(){
    std::string payload_str = "bool=true;int=111;int_arr=[1,2,3];str=\"test_str\";str_arr=[\"str1\",\"str2\",\"str3\"];test_null=null;";

    auto test_payload = payload::parse(payload_str);

    printf("Test parsed payload: \t\t%s\n", test_payload->construct().c_str());
}

void dispatch_connections(){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(configuration::get_file("config")->GetLongValue("Connection", "port"));

    int err = bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    if (err != 0){
        LOGGER->error(fmt::format("Server socket binding ended with error code: {}", err));
        return;
    }

    err = listen(server_fd, 10);

    if (err != 0){
        LOGGER->error(fmt::format("Server socket listen ended with error code: {}", err));
        return;
    }


}

int main(){
    log4cxx::xml::DOMConfigurator::configure("config/logging/log4cxx.xml");
    configuration::init({
                                {"config/config.ini", "config"}
                        });
    dispatch_connections();
    LOGGER->debug(configuration::get_instance()->to_string());
    LOGGER->debug(std::to_string(configuration::get_file("config")->GetLongValue("Connection", "port")));
    return 0;
}