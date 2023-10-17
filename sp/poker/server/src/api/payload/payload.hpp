#pragma once

#include <unordered_map>
#include <string>
#include <memory>

class Payload{
protected:
    std::unique_ptr<std::unordered_map<std::string, std::shared_ptr<void>>> data;

public:
    Payload();

    std::shared_ptr<void> getData(const std::string& key);

    void setData(const std::string& key, std::shared_ptr<void> value);
};