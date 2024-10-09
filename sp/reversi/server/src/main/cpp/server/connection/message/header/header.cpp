#include <stdexcept>
#include <sstream>
#include <iomanip>

#include "header.hpp"
#include "../../consts/consts.hpp"
#include "fmt/format.h"


header::header(const std::string &identifier, type type, subtype subtype, status status, size_t length) : identifier(
        identifier), _type(type), _subtype(subtype), _status(status), length(length) {}

const std::string &header::get_identifier() const {
    return identifier;
}

const type &header::get_type() const {
    return _type;
}

const subtype &header::get_subtype() const {
    return _subtype;
}

const status &header::get_status() const {
    return _status;
}

size_t header::get_length() const {
    return length;
}

void header::set_identifier(const std::string &identifier) {
    header::identifier = identifier;
}

void header::set_type(const type &type) {
    _type = type;
}

void header::set_subtype(const subtype &subtype) {
    _subtype = subtype;
}

void header::set_status(const status &status) {
    _status = status;
}

void header::set_length(size_t length) {
    header::length = length;
}

bool header::check_values() const {
    return length >= 0 && length <= constants::MSG_MAX_LENGTH && _status != status::NULL_STATUS && _subtype != subtype::NULL_SUBTYPE && _type != type::NULL_TYPE;
}

std::shared_ptr<header> header::extract(const std::string &message) {
    if (message.length() < constants::MSG_HEADER_LENGTH){
        throw std::invalid_argument("Message _header is too short!");
    }

    std::string msg_identifier = message.substr(constants::MSG_IDENTIFIER_FIELD_POS, constants::MSG_IDENTIFIER_FIELD_LENGTH);
    int msg_length = std::stoi(message.substr(constants::MSG_LENGTH_FIELD_POS, constants::MSG_LENGTH_FIELD_LENGTH));
    int msg_type_int = std::stoi(message.substr(constants::MSG_TYPE_FIELD_POS, constants::MSG_TYPE_FIELD_LENGTH));
    int msg_subtype_int = std::stoi(message.substr(constants::MSG_SUBTYPE_FIELD_POS, constants::MSG_SUBTYPE_FIELD_LENGTH));
    int msg_status_int = std::stoi(message.substr(constants::MSG_STATUS_FIELD_POS, constants::MSG_STATUS_FIELD_LENGTH));

    type msg_type = type_mapper::get_type(msg_type_int);
    subtype msg_subtype = subtype_mapper::get_subtype(msg_subtype_int);
    status msg_status = status_mapper::get_status(msg_status_int);
    return std::make_shared<header>(msg_identifier, msg_type, msg_subtype, msg_status, msg_length);
}

std::string header::construct() const{
    if (!check_values()) {
        throw std::logic_error("Incomplete _header!");
    }

    std::ostringstream oss;
    oss << identifier;
    oss << std::setw(4) << std::setfill('0') << get_length();
    oss << std::setw(1) << type_mapper::get_id(get_type());
    oss << std::setw(2) << subtype_mapper::get_id(get_subtype());
    oss << std::setw(3) << status_mapper::get_id(get_status());

    return oss.str();
}

header::header() {}

header::header(const std::string &identifier, type type, subtype subtype, status status) : identifier(identifier),
                                                                                           _type(type),
                                                                                           _subtype(subtype),
                                                                                           _status(status) {}

std::string header::to_string() const {
    return fmt::format("Header [identifier={}, type={}, subtype={}, status={}, length={}]", identifier, type_mapper::get_id(_type), subtype_mapper::get_id(_subtype), status_mapper::get_id(_status), this->length);
}




