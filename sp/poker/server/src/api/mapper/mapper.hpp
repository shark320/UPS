#pragma once

#include "../request/request.hpp"
#include "../type.hpp"
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::shared_ptr<Request> mapRequest(const std::shared_ptr<std::string>& message){
    json jsonData = json::parse(*message);
    std::string type = jsonData["type"];
    std::string subType = jsonData["subtype"];

    auto request = std::make_shared<Request>();
    request->setType(mapType(type));
    request->setSubType(mapSubType(subType));

    return request;
}