#include "payload.hpp"
#include <iostream>

Payload::Payload() {
    // Constructor implementation, if needed
}

void Payload::setValue(const std::string& key, const std::shared_ptr<void*>& value) {
    data[key] = value;
}

std::shared_ptr<void*> Payload::getValue(const std::string& key) {
    return data[key];
}

std::map<std::string, std::shared_ptr<void*>> Payload::getData() {
    return data;
}

std::string Payload::toString() {
    std::string result = "Payload{data={";
    for (const auto& entry : data) {
        result += entry.first + "=" + std::to_string(entry.second) + ", ";
    }
    if (!data.empty()) {
        result = result.substr(0, result.length() - 2); // Remove the last ", "
    }
    result += "}}";
    return result;
}
