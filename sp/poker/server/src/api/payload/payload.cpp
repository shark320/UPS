//
// Created by vladi on 16.10.2023.
//
#include "payload.hpp"
#include <memory>
#include <utility>


Payload::Payload() {
    data = std::make_unique<std::unordered_map<std::string, std::shared_ptr<void>>>();
}

std::shared_ptr<void> Payload::getData(const std::string& key) {
    return  (*data)[key];
}

void Payload::setData(const std::string& key, std::shared_ptr<void> value) {
    (*data)[key]=std::move(value);
}

