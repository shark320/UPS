#pragma once

#include <map>
#include <string>
#include <memory>
#include "../../base/base.hpp"

class payload: public object{
public:
    payload();

    void set_value(const std::string& key, const std::shared_ptr<object>& value);
    std::shared_ptr<object> get_value(const std::string& key);
    std::shared_ptr<std::map<std::string, std::shared_ptr<object>>> get_data();

    std::string to_string();

private:
    std::shared_ptr<std::map<std::string, std::shared_ptr<object>>> data;
};

