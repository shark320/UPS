#pragma once
#include <string>
#include <unordered_map>

enum class Type{

    POST,
    GET
};

Type mapType(const std::string& typeString){
    static const std::unordered_map<std::string, Type> typeMap = {
            {"POST", Type::POST},
            {"GET", Type::GET}
    };
    auto it = typeMap.find(typeString);
    if (it != typeMap.end()) {
        return it->second;
    } else {
        // Default value if the string is not found
        return Type::GET; // You can choose any suitable default value
    }
}

enum class SubType{
    PING
};

SubType mapSubType(const std::string& typeString){
    static const std::unordered_map<std::string, SubType> typeMap = {
            {"PING", SubType::PING}
    };
    auto it = typeMap.find(typeString);
    if (it != typeMap.end()) {
        return it->second;
    } else {
        // Default value if the string is not found
        return SubType::PING; // You can choose any suitable default value
    }
}