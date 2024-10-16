#include <sstream>
#include <algorithm>
#include "utils.hpp"

static const char PATH_DELIMITER = '/';

static const char REVERSE_PATH_DELIMITER = '\\';


std::shared_ptr<str_vector> split_str(const std::string &str, char delimiter) {
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

int count_digits(size_t number) {
    int count = 0;
    if (number == 0) {
        return 1;
    }
    while (number > 0) {
        number /= 10;
        count++;
    }

    return count;
}

bool is_whitespaces_only(const std::string &str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); });
}

std::string format_timestamp(const std::shared_ptr<std::chrono::steady_clock::time_point>& timestamp){
    // Obtain the current time from system_clock
    auto now_system = std::chrono::system_clock::now();
    auto now_steady = std::chrono::steady_clock::now();

    // Calculate the time difference between now on steady clock and given steady time point
    auto diff = *timestamp - now_steady;

    // Get the corresponding system_clock::time_point
    auto system_tp = now_system + diff;

    // Convert to time_t for use with std::put_time
    std::time_t time = std::chrono::system_clock::to_time_t(system_tp);

    // Format the time into a string
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");

    return ss.str();
}
