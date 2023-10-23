#pragma once

#include <map>
#include <string>
#include <memory>

class Payload {
public:
    Payload();

    void setValue(const std::string& key, const std::shared_ptr<void*>& value);
    std::shared_ptr<void*> getValue(const std::string& key);
    std::map<std::string, std::shared_ptr<void*>> getData();

    std::string toString();

private:
    std::map<std::string, std::shared_ptr<void*>> data;
};

