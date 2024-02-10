#include "message.hpp"


const std::shared_ptr<header> &message::get_header() const {
    return _header;
}

void message::set_header(const std::shared_ptr<header> &header) {
    message::_header = header;
}

const std::shared_ptr<payload> &message::get_payload() const {
    return _payload;
}

void message::set_payload(const std::shared_ptr<payload> &payload) {
    message::_payload = payload;
}
