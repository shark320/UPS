#include "base.hpp"
#include <stdexcept>
#include <utility>

object::~object() = default;

integer::integer() {
    this->_value = 0;
}

integer::integer(const integer &other) {
    this->_value = other._value;
}

integer::integer(int _value){
    this->_value = _value;
}

std::string integer::to_string() const {
    return std::to_string(this->_value);
}

integer integer::operator+(const integer &other) const {
    return integer(this->_value + other._value);
}

integer integer::operator-(const integer &other) const {
    return integer(this->_value - other._value);
}

integer integer::operator*(const integer &other) const {
    return integer(this->_value * other._value);
}

integer integer::operator/(const integer &other) const {
    if (this->_value == 0){
        throw std::invalid_argument("Division by zero!");
    }
    return integer(this->_value / other._value);
}

bool integer::operator==(const integer &other) const {
    return this->_value == other._value;
}

bool integer::operator!=(const integer &other) const {
    return this->_value != other._value;
}

integer &integer::operator=(const integer &other) {
    if (this != &other){
        this->_value = other._value;
    }

    return *this;
}

integer &integer::operator=(const int &_value) {
    this->_value = _value;
    return *this;
}

int integer::value() {
    return this->_value;
}

string::string(std::string str): std::string(std::move(str)) {}


std::string string::to_string() const {
    return *this;
}

std::string objects_vector::to_string() const {
    if (this->empty()){
        return "[]";
    }
    std::string result = "[";

    for (const std::shared_ptr<object>& item : *this){
        result += item->to_string() + ", ";
    }

    result.replace(result.length()-2, 1, "]");
    result.replace(result.length()-1, 1, "");

    return result;
}

std::string object::to_string() const {
    return {};
}



boolean::boolean() {
    this->_value = 0;
}

boolean::boolean(const boolean &other) {
    this->_value = other._value;
}

boolean::boolean(bool _value){
    this->_value = _value;
}

std::string boolean::to_string() const {
    if (this->_value){
        return "true";
    }else{
        return "false";
    }
}

bool boolean::operator==(const boolean &other) const {
    return this->_value == other._value;
}

bool boolean::operator!=(const boolean &other) const {
    return this->_value != other._value;
}

boolean &boolean::operator=(const boolean &other) {
    if (this != &other){
        this->_value = other._value;
    }

    return *this;
}

boolean &boolean::operator=(const bool &_value) {
    this->_value = _value;
    return *this;
}

bool boolean::value() {
    return this->_value;
}
