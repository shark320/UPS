#pragma once

#include <string>
#include <vector>
#include <array>
#include <memory>

/**
 * @brief vector of std::string
 */
using str_vector = std::vector<std::string>;

/**
 * @brief Splits the string by the provided delimiter.
 * @param str string to split.
 * @param delimiter delimiter to split by.
 * @return pointer to the vector of strings (split tokens)
 */
std::shared_ptr<str_vector> split_str(const std::string& str, char delimiter);

/**
     * @brief Parses a file path from the string
     * @param file_path string to parse
     * @return pointer to an array[2] of parsed values. [0] - directory path, [1] - file name.
     */
static std::shared_ptr<std::array<std::string, 2>> parse_file_path(const std::string& file_path);

int count_digits(size_t number);