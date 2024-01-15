#include <sstream>

#include "utils.hpp"

std::shared_ptr<str_vector> split_str(const std::string& str, char delimiter){
    auto tokens = std::make_shared<str_vector>();
    std::istringstream str_stream(str);
    std::string token;
    while (std::getline(str_stream, token, delimiter)) {
        tokens->push_back(token);
    }

    return tokens;
}
