#include <logger.h>
#include <fstream>
#include "configuration.hpp"
#include "fmt/format.h"

std::shared_ptr<configuration> configuration::INSTANCE = nullptr;


static const auto LOGGER = log4cxx::Logger::getLogger("configuration");

configuration::configuration() {
    this->files_map = std::make_shared<std::unordered_map<std::string, std::shared_ptr<CSimpleIniA>>>();
}



std::string configuration::to_string() {
    std::string result = "configuration: ";
    if (INSTANCE == nullptr){
        result += "nullptr";
    }else{
        for (auto pair : *INSTANCE->files_map){
            result += pair.first + ", ";
        }
    }
    return result;
}

std::shared_ptr<configuration> configuration::get_instance() {
    return INSTANCE;
}

std::shared_ptr<CSimpleIniA> configuration::get_file(std::string alias) {
    if (INSTANCE == nullptr){
        LOGGER->error("Configuration has not been initialized!");
        return nullptr;
    }
    auto it = INSTANCE->files_map->find(alias);
    if (it == INSTANCE->files_map->end()){
        LOGGER->error(fmt::format("File '{}' was not found!", alias));
        return nullptr;
    }
    return it->second;
}

void configuration::init(std::initializer_list<std::pair<std::string, std::string>> files) {
    LOGGER->debug("Initializing configuration.");
    struct make_shared_enabler : public configuration {
        make_shared_enabler() : configuration() {}
    };
    INSTANCE = std::make_shared<make_shared_enabler>();
    for (auto const& file_name: files){
        load_file(file_name.first, file_name.second);
    }
}

void configuration::load_file(std::string file_name, const std::string& alias) {
    if (INSTANCE == nullptr){
        LOGGER->error("Configuration has not been initialized!");
        return;
    }
    std::ifstream file(file_name);
    auto ini_obj = std::make_shared<CSimpleIniA>();
    if (SI_Error rc = ini_obj->LoadFile(file_name.c_str()); rc < 0){
        LOGGER->error(fmt::format("Configuration file '{}' does not exist or could not be opened", file_name));
        return;
    }
    INSTANCE->files_map->insert({alias,ini_obj});
    file.close();
}


