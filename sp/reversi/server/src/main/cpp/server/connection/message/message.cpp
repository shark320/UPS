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

message::message(std::shared_ptr<header> header_, std::shared_ptr<payload> payload_) {
    this->_header = header_;
    this->_payload = payload_;
}

std::string message::to_string() {
    return "Message: {" + _header->to_string() + "; " + _payload->to_string() + "}";
}

std::string message::construct() {
    std::string payload_str = _payload->construct();
    _header->set_length(payload_str.length());
    std::string header_str = _header->construct();

    return header_str + payload_str;
}
