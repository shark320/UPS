#pragma once

#include <string>
#include <vector>
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