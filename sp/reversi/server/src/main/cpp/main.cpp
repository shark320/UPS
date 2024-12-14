#include <iostream>
#include "../../test/cpp/test.hpp"
#include "config/configuration.hpp"
#include "server/config/server_config.hpp"
#include "server/server.hpp"
#include "fmt/format.h"
#include "reversi/config/game_config.hpp"
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/xml/domconfigurator.h>

void increase(int& a){
    a = 10;
}

static auto LOGGER = log4cxx::Logger::getLogger("main");

void start(int argc, char *argv[]){
    configuration::init({
                                {"config/config.ini", "config"}
                        });
    if (argc > 1 && std::string(argv[1]) == "test") {
        test_suit::test();
    }else{
        auto _server_config = std::make_shared<server_config>(configuration::get_file("config"));
        auto _connection_config = std::make_shared<connection_config>(configuration::get_file("config"));
        auto _game_config = std::make_shared<game_config>(configuration::get_file("config"));
        LOGGER->debug(fmt::format("Server config: {}", _server_config->to_string()));
        LOGGER->debug(fmt::format("Connection config: {}", _connection_config->to_string()));
        auto _message_manager = std::make_shared<message_manager>(_connection_config, _game_config);
        std::shared_ptr<server> _server = std::make_shared<server>(_server_config, _message_manager);
        _server->start();
    }
}

int main(int argc, char *argv[]) {
    try {
        log4cxx::xml::DOMConfigurator::configure("config/logging/log4cxx.xml");
        start(argc, argv);
        return 0;
    }catch(const std::exception& ex){
        std::cerr << "Exception caught: " << ex.what() << std::endl;
        return -1;
    }catch (...){
        std::cerr << "Unknown exception caught!" << std::endl;
        return -1;
    }


}
