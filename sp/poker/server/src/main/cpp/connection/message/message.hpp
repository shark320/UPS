#pragma once

#include "payload/payload.hpp"
#include "header/header.hpp"
#include <memory>

class message{

private:

    std::shared_ptr<header> _header;

    std::shared_ptr<payload> _payload;

public:

    message(std::string msg);

    message();

    [[nodiscard]] const std::shared_ptr<header> &get_header() const;

    void set_header(const std::shared_ptr<header> &header);

    [[nodiscard]] const std::shared_ptr<payload> &get_payload() const;

    void set_payload(const std::shared_ptr<payload> &payload);

};