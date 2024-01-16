#pragma once

#include <map>
#include <string>
#include <memory>
#include "../../base/base.hpp"

class payload: public object{
public:
    payload();

    void set_value(const std::string& key, const std::shared_ptr<object>& value);
    std::shared_ptr<object> get_value(const std::string& key);
    std::shared_ptr<std::map<std::string, std::shared_ptr<object>>> get_data();

    std::string to_string();

    static std::shared_ptr<payload> extract(const std::string& str);

private:
    std::shared_ptr<std::map<std::string, std::shared_ptr<object>>> data;

    static std::shared_ptr<vector> parse_int_list(const std::string& value);

    static std::shared_ptr<vector>  parse_string_list(const std::string& value);

    static std::shared_ptr<vector> parse_list(const std::string& value);

    static std::shared_ptr<string> parse_string(const std::string& value);

    static std::shared_ptr<integer> parse_int(const std::string& value);

    static std::shared_ptr<boolean> parse_boolean(const std::string& value);

    static std::shared_ptr<object> parse_value(const std::string& value);

    static void parse_token(const std::string& token, const std::shared_ptr<payload>& _payload);

    static std::shared_ptr<payload> parse(const std::string& str);

    static std::string map_string(const std::shared_ptr<object>& str);

    static std::string map_string_list(const std::shared_ptr<vector>& strings_list);

    static std::string map_int(const std::shared_ptr<object>& value);

    static std::string map_ints_list(const std::shared_ptr<vector>& integers_list);

    static std::string map_list(const std::shared_ptr<vector>& list);

    static std::string map_boolean(const std::shared_ptr<boolean>& value);

    static std::string map_object(const std::shared_ptr<object>& obj);

    static std::string map(const std::shared_ptr<payload>& _payload);
};

