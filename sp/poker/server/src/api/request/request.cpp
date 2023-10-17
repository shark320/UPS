//
// Created by vladi on 16.10.2023.
//
#include "request.hpp"

void Request::setType(Type type) {
    Request::type = type;
}

void Request::setSubType(SubType subType) {
    Request::subType = subType;
}

Type Request::getType() const {
    return type;
}

SubType Request::getSubType() const {
    return subType;
}

const std::shared_ptr<Payload> & Request::getPayload() const {
    return payload;
}

void Request::setPayload(const std::shared_ptr<Payload> &payload) {
    Request::payload = payload;
}
