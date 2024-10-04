#pragma once

#include <memory>
#include <unordered_map>
#include "SimpleIni.h"

class configuration {

private:
    static std::shared_ptr<configuration> INSTANCE;

    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<CSimpleIniA>>> files_map;

    configuration();

public:

    static void init(std::initializer_list<std::pair<std::string, std::string>> files);

    static std::shared_ptr<configuration> get_instance();

    static std::shared_ptr<CSimpleIniA> get_file(std::string alias);

    static void load_file(std::string file_name, const std::string& alias);

    std::string to_string();
};

