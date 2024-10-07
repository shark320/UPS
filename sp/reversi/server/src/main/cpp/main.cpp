#include <iostream>
#include "../../test/cpp/test.hpp"
#include "config/configuration.hpp"
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/xml/domconfigurator.h>

void increase(int& a){
    a = 10;
}

int main(int argc, char *argv[]) {
    log4cxx::xml::DOMConfigurator::configure("config/logging/log4cxx.xml");
    configuration::init({
                                {"config/config.ini", "config"}
                        });
    if (argc > 1 && std::string(argv[1]) == "test") {
        test_suit::test();
    }else{
        //TODO:
    }
    return 0;
}
