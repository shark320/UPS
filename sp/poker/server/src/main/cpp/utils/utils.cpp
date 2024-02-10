#include <sstream>
#include <algorithm>
#include "utils.hpp"

static const char PATH_DELIMITER = '/';

static const char REVERSE_PATH_DELIMITER = '\\';


std::shared_ptr<str_vector> split_str(const std::string& str, char delimiter){
    auto tokens = std::make_shared<str_vector>();
    std::istringstream str_stream(str);
    std::string token;
    while (std::getline(str_stream, token, delimiter)) {
        tokens->push_back(token);
    }

    return tokens;
}

std::shared_ptr<std::array<std::string, 2>> parse_file_path(const std::string &file_path) {
    auto result = std::make_shared<std::array<std::string, 2>>();
    std::string path_copy = file_path;
    std::replace(path_copy.begin(), path_copy.end(), REVERSE_PATH_DELIMITER, PATH_DELIMITER);
    if (size_t last_delim = path_copy.find_last_of(PATH_DELIMITER); last_delim == std::string::npos) {
        result->at(0) = ".";
        result->at(1) = path_copy;
    } else {
        result->at(0) = path_copy.substr(0, last_delim);
        result->at(1) = path_copy.substr(last_delim + 1);
    }

    return result;
}
