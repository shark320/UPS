
#include <regex>
#include "payload.hpp"
#include "../../../../utils/utils.hpp"
#include "../../consts/consts.hpp"

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

payload::payload(const payload* other){
    this->data = other->data;
}

void payload::set_value(const std::string &key, const std::shared_ptr<object> &value) {
    if (!validate_value(value)){
        throw std::invalid_argument("Provided value is invalid and can not be set!");
    }
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

std::shared_ptr<objects_vector> payload::parse_int_list(const std::string& value) {
    auto result = std::make_shared<objects_vector>();
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

std::shared_ptr<objects_vector> payload::parse_string_list(const std::string& value) {
    auto result = std::make_shared<objects_vector>();
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

std::shared_ptr<objects_vector> payload::parse_list(const std::string& value) {
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
        return nullptr;
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

//std::shared_ptr<payload> payload::extract(const std::string& str){
//    if (str.length() <= constants::MSG_HEADER_LENGTH){
//        return std::make_shared<payload>();
//    }
//
//    std::string payload_str = str.substr(constants::MSG_HEADER_LENGTH);
//    return parse(payload_str);
//}

std::string payload::map_string(const std::shared_ptr<string> &str){
    if (str == nullptr){
        return "null";
    }

    return '"' + str->to_string() + '"';
}

std::string payload::map_string_list(const std::shared_ptr<objects_vector>& strings_list){
    if (strings_list == nullptr || strings_list->empty()){
        return "";
    }
    std::ostringstream oss;

    for (const auto& str : *strings_list) {
        if (std::shared_ptr<string> str_ptr = std::dynamic_pointer_cast<string>(str)){
            oss << map_string(str_ptr) << ',';
        }else{
            throw std::invalid_argument("Vector contains not string type object!");
        }
    }
    std::string result = oss.str();
    result.pop_back();
    return result;
}

std::string payload::map_int(const std::shared_ptr<integer> &value) {
    if (value == nullptr){
        return "null";
    }

    return value->to_string();
}

std::string payload::map_ints_list(const std::shared_ptr<objects_vector> &integers_list) {
    if (integers_list == nullptr || integers_list->empty()){
        return "";
    }
    std::ostringstream oss;

    for (const auto& item : *integers_list) {
        if (std::shared_ptr<integer> int_ptr = std::dynamic_pointer_cast<integer>(item)){
            oss << map_int(int_ptr) << ',';
        }else{
            throw std::invalid_argument("Vector contains not integer type object!");
        }

    }
    std::string result = oss.str();
    result.pop_back();
    return result;
}

std::string payload::map_list(const std::shared_ptr<objects_vector>& list){
    if (list == nullptr || list->empty()) {
        return "null";
    }
    std::string result;
    const auto& first_element = list->at(0);
    if (std::shared_ptr<integer> int_ptr = std::dynamic_pointer_cast<integer>(first_element)) {
        result = map_ints_list(list);
    } else if (std::shared_ptr<string> str_ptr = std::dynamic_pointer_cast<string>(first_element)) {
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
    if (obj == nullptr){
        return "null";
    }
    auto obj_tmp = *obj;
    if (std::shared_ptr<string> str_ptr = std::dynamic_pointer_cast<string>(obj)){
        return map_string(str_ptr);
    }
    if (std::shared_ptr<integer> int_ptr = std::dynamic_pointer_cast<integer>(obj)){
        return map_int(int_ptr);
    }
    if (std::shared_ptr<objects_vector> vector_ptr = std::dynamic_pointer_cast<objects_vector>(obj)){
        return map_list(vector_ptr);
    }
    if (std::shared_ptr<boolean> bool_ptr = std::dynamic_pointer_cast<boolean>(obj)){
        return map_boolean(bool_ptr);
    }
    return "";
}

std::string payload::map(const std::shared_ptr<payload> &_payload) {
    if (_payload->get_data()->empty()){
        return "";
    }
    std::ostringstream oss;
    for (auto const& it : *(_payload->get_data())){
        oss << it.first << '=' << map_object(it.second) << SEPARATOR;
    }
    oss.seekp(-1, std::ios_base::end); // Move back to remove the last separator
    return oss.str();
}

std::string payload::construct() const{
    //TODO: replace with function overloading
    return map(std::make_shared<payload>(this));
}

bool payload::validate_str_value(const std::shared_ptr<string> value) {
    if (value == nullptr){
        return true;
    }
    return value->find(SEPARATOR) == std::string::npos;
}

bool payload::validate_str_list_value(const std::shared_ptr<objects_vector>& value){
    if (value == nullptr || value->empty()){
        return true;
    }
    for (auto str : *value){
        if (auto str_ptr = std::dynamic_pointer_cast<string>(str)){
            if (!validate_str_value(str_ptr)){
                return false;
            }
        } else {
            throw std::invalid_argument("Vector contains not string type object!");
        }
    }

    return true;
}

bool payload::validate_list_value(const std::shared_ptr<objects_vector>& value) {
    if (value == nullptr || value->empty()){
        return true;
    }
    auto first_item = value->at(0);
    if (auto str_ptr = std::dynamic_pointer_cast<string>(first_item)){
        return validate_str_list_value(value);
    }

    //other types are valid
    return true;
}

bool payload::validate_value(const std::shared_ptr<object>& value){
    if (value == nullptr){
        return true;
    }

    if (auto str_ptr = std::dynamic_pointer_cast<string>(value)){
        return validate_str_value(str_ptr);
    }
    if (auto vector_ptr = std::dynamic_pointer_cast<objects_vector>(value)){
        return validate_list_value(vector_ptr);
    }

    //other types are valid
    return true;
}










