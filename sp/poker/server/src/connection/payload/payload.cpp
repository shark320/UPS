#include "payload.hpp"

payload::payload() {
    this->data = std::make_shared<std::map<std::string, std::shared_ptr<object>>>();
}

void payload::set_value(const std::string &key, const std::shared_ptr<object> &value) {
    (*this->data)[key] = value;
}

std::shared_ptr<object> payload::get_value(const std::string &key) {
    auto it = this->data->find(key);
    if (it == this->data->end()){
        return nullptr;
    }

    return it->second;
}

std::shared_ptr<std::map<std::string, std::shared_ptr<object>>> payload::get_data() {
    return this->data;
}

std::string payload::to_string() {
    std::string result = "[";
    for (auto const& item : *this->data){
        result += item.first + "=" + item.second->to_string() + ", ";
    }

    result.replace(result.length()-2, 1, "]");
    result.replace(result.length()-1, 1, "");

    return result;
}






