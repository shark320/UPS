
#include <regex>
#include "payload.hpp"
#include "../../utils/utils.hpp"
#include "../consts/consts.hpp"

static const std::regex LIST_PATTERN = std::regex(R"(^\[(("[^"]*(?:","[^"]*)*")|\d+(?:,\d+)*)\]$)");
static const std::regex LIST_INT_PATTERN = std::regex(R"(^\[(\d+(?:,\d+)*)\]$)");
static const std::regex LIST_STRING_PATTERN = std::regex(R"(^\[("[^"]*(?:","[^"]*)*")\]$)");
static const std::regex STRING_PATTERN = std::regex("^\"([^\"]*)\"$");
static const std::regex INT_PATTERN = std::regex("^\\d+$");
static const std::regex BOOL_PATTERN = std::regex("^(true|false)$");
static const std::regex NULL_PATTERN = std::regex("^(null)$");

static const std::string LIST_SEPARATOR = ",";
static const std::string KEY_SEPARATOR = "=";
static const char SEPARATOR = ';';

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
    std::string result = "payload={";
    for (auto const& item : *this->data){
        result += item.first + "=" + item.second->to_string() + ", ";
    }

    if (result.length() > 1){
        result.replace(result.length()-2, 1, "}");
        result.replace(result.length()-1, 1, "");
    } else{
        result += "}";
    }


    return result;
}

std::shared_ptr<vector> payload::parse_int_list(const std::string& value) {
    auto result = std::make_shared<vector>();
    std::smatch match;
    std::regex_search(value, match, LIST_PATTERN);
    std::string tokens = match[1];
    size_t pos = 0;
    std::string token;
    while ((pos = tokens.find(LIST_SEPARATOR)) != std::string::npos) {
        token = tokens.substr(0, pos);
        result->push_back(parse_int(token));
        tokens.erase(0, pos + LIST_SEPARATOR.length());
    }
    result->push_back(parse_int(tokens)); // Last token
    return result;
}

std::shared_ptr<vector> payload::parse_string_list(const std::string& value) {
    auto result = std::make_shared<vector>();
    std::smatch match;
    std::regex_search(value, match, LIST_STRING_PATTERN);
    std::string tokens = match[1];
    size_t pos = 0;
    std::string token;
    while ((pos = tokens.find(LIST_SEPARATOR)) != std::string::npos) {
        token = tokens.substr(0, pos);
        result->push_back(parse_string(token));
        tokens.erase(0, pos + LIST_SEPARATOR.length());
    }
    result->push_back(parse_string(tokens)); // Last token
    return result;
}

std::shared_ptr<vector> payload::parse_list(const std::string& value) {
    std::smatch str_list_match;
    std::smatch int_list_match;
    if (std::regex_search(value, str_list_match, LIST_STRING_PATTERN)) {
        return parse_string_list(value);
    } else if (std::regex_search(value, int_list_match, LIST_INT_PATTERN)) {
        return parse_int_list(value);
    } else {
        throw std::invalid_argument("Invalid list type: " + value);
    }
}

std::shared_ptr<string> payload::parse_string(const std::string& value) {
    return std::make_shared<string>(std::string(value.substr(1, value.length() - 2)));
}

std::shared_ptr<integer> payload::parse_int(const std::string& value) {
    return std::make_shared<integer>(std::stoi(value));
}

std::shared_ptr<boolean> payload::parse_boolean(const std::string& value) {
    return std::make_shared<boolean>(value == "true");
}

std::shared_ptr<object> payload::parse_value(const std::string& value) {
    std::smatch str_match;
    std::smatch int_match;
    std::smatch bool_match;
    std::smatch null_match;
    std::smatch list_match;
    if (std::regex_search(value, str_match, STRING_PATTERN)) {
        return parse_string(value);
    } else if (std::regex_search(value, int_match, INT_PATTERN)) {
        return parse_int(value);
    } else if (std::regex_search(value, bool_match, BOOL_PATTERN)) {
        return parse_boolean(value);
    } else if (std::regex_search(value, null_match, NULL_PATTERN)) {
        return std::make_shared<object>();
    } else if (std::regex_search(value, list_match, LIST_PATTERN)){
        return parse_list(value);
    }else {
        throw std::invalid_argument("Invalid value: " + value);
    }
}

void payload::parse_token(const std::string& token, const std::shared_ptr<payload>& _payload) {
    size_t pos = token.find(KEY_SEPARATOR);
    if (pos != std::string::npos) {
        std::string key = token.substr(0, pos);
        std::string value = token.substr(pos + KEY_SEPARATOR.length());
        _payload->set_value(key, parse_value(value));
    } else {
        throw std::invalid_argument("Unable to parse token: " + token);
    }
}

std::shared_ptr<payload> payload::parse(const std::string& str) {
    auto _payload = std::make_shared<payload>();
    auto tokens = split_str(str, SEPARATOR);
    for (auto const& token: *tokens){
        parse_token(token, _payload);
    }
    return _payload;
}

std::shared_ptr<payload> payload::extract(const std::string& str){
    if (str.length() <= constants::MSG_HEADER_LENGTH){
        return std::make_shared<payload>();
    }

    std::string payload_str = str.substr(constants::MSG_HEADER_LENGTH);
    return parse(payload_str);
}

std::string payload::map_string(const std::shared_ptr<object>& str){
    if (str == nullptr){
        return "null";
    }

    return '"' + str->to_string() + '"';
}

std::string payload::map_string_list(const std::shared_ptr<vector>& strings_list){
    if (strings_list == nullptr){
        return "null";
    }
    std::ostringstream oss;

    for (const auto& str : *strings_list) {
        oss << map_string(str) << ',';
    }
    oss.seekp(-1, std::ios_base::end); // Move back to remove the last comma
    return oss.str();
}

std::string payload::map_int(const std::shared_ptr<object> &value) {
    if (value == nullptr){
        return "null";
    }

    return value->to_string();
}

std::string payload::map_ints_list(const std::shared_ptr<vector> &integers_list) {
    if (integers_list == nullptr){
        return "null";
    }
    std::ostringstream oss;

    for (const auto& str : *integers_list) {
        oss << map_int(str) << ',';
    }
    oss.seekp(-1, std::ios_base::end); // Move back to remove the last comma
    return oss.str();
}

std::string payload::map_list(const std::shared_ptr<vector>& list){
    if (list == nullptr) {
        return "null";
    }
    if (list->empty()){
        return "[]";
    }
    std::string result;
    const auto& first_element = *(list->at(0));
    if (typeid(first_element) == typeid(integer)) {
        result = map_ints_list(list);
    } else if (typeid(first_element) == typeid(string)) {
        result = map_string_list(list);
    }else {
        throw std::invalid_argument("Unknown list items type to map.");
    }
    return "[" + result + "]";
}

std::string payload::map_boolean(const std::shared_ptr<boolean> &value) {
    return value->value() ? "true" : "false";
}

std::string payload::map_object(const std::shared_ptr<object>& obj){

}










