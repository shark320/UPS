#pragma once

#include "../type.hpp"
#include "../payload/payload.hpp"
#include <memory>

class Request {
protected:
    Type type;
    SubType subType;
    std::shared_ptr<Payload> payload;

public:

    void setType(Type type);

    void setSubType(SubType subType);

    Type getType() const;

    SubType getSubType() const;

    const std::shared_ptr<Payload> &getPayload() const;

    void setPayload(const std::shared_ptr<Payload> &payload);

};