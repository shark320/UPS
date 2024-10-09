#include <iostream>
#include "../../test/cpp/test.hpp"
#include "config/configuration.hpp"
#include "server/config/server_config.hpp"
#include "server/server.hpp"
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/xml/domconfigurator.h>

void increase(int& a){
    a = 10;
}

static auto LOGGER = log4cxx::Logger::getLogger("main");

int main(int argc, char *argv[]) {
    log4cxx::xml::DOMConfigurator::configure("config/logging/log4cxx.xml");
    configuration::init({
                                {"config/config.ini", "config"}
                        });
    if (argc > 1 && std::string(argv[1]) == "test") {
        test_suit::test();
    }else{
        std::shared_ptr<server_config> _server_config = std::make_shared<server_config>(configuration::get_file("config"));
        LOGGER->debug(_server_config->to_string());
        std::shared_ptr<server> _server = std::make_shared<server>(_server_config);
        _server->start();
    }
    return 0;
}
