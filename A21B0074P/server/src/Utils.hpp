#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * @brief Logs a message to the server's log.
 * @param message The message to be logged.
 */
void log(const string& message);

/**
 * @brief Trims the end of line characters from a string.
 * @param str The input string.
 * @return A string with end of line characters removed.
 */
string trimEndOfLine(const string& str);

/**
 * @brief Compares two strings after trimming whitespace.
 * @param lhs The first string to compare.
 * @param rhs The second string to compare.
 * @return True if the trimmed strings are equal, false otherwise.
 */
bool trimCompare(const string& lhs, const string& rhs);

/**
 * @brief Trims leading and trailing whitespace from a string.
 * @param str The input string.
 * @return A string with leading and trailing whitespace removed.
 */
string trim(const string& str);

/**
 * @brief Splits a message into a vector of substrings using a delimiter.
 * @param message The input message to be split.
 * @param delimiter The character used to split the message.
 * @return A vector of substrings obtained by splitting the message.
 */
vector<string> splitMessage(const string& message, char delimiter);

/**
 * @brief Generates a response message with a specified opcode, status, and message.
 * @param opcode The operation code for the response.
 * @param status The status of the response (true for success, false for failure).
 * @param message The message to include in the response.
 * @return A formatted response message string.
 */
string generateResponse(const string& opcode, bool status, const string& message);

/**
 * @brief Generates a header for a message with a specified length.
 * @param length The length of the message.
 * @return A formatted header string.
 */
string generateHeader(int length);

/**
 * @brief Generates a positive response message with a specified opcode and message.
 * @param opcode The operation code for the positive response.
 * @param message The message to include in the response.
 * @return A formatted positive response message string.
 */
string generatePositiveResponse(const string& opcode, const string& message);

/**
 * @brief Generates a negative response message with a specified opcode and message.
 * @param opcode The operation code for the negative response.
 * @param message The message to include in the response.
 * @return A formatted negative response message string.
 */
string generateNegativeResponse(const string& opcode, const string& message);

/**
 * @brief Checks if a string contains only control characters.
 * @param str The string to check.
 * @return True if the string contains only control characters, false otherwise.
 */
bool containsOnlyControlChars(const std::string& str);

/**
 * @brief Checks if a C-style string contains only control characters.
 * @param str The C-style string to check.
 * @return True if the string contains only control characters, false otherwise.
 */
bool containsOnlyControlChars(const char* str);

/**
 * @brief Checks if all components in a vector of strings are valid.
 * @param components The vector of strings to check.
 * @return True if all components are valid, false otherwise.
 */
bool areAllComponentsValid(const std::vector<std::string>& components);

/**
 * @brief Checks if a string is a valid binary string.
 * @param str The string to check.
 * @return True if the string is a valid binary string, false otherwise.
 */
bool isBinaryString(const string& str);

#endif //SERVER_UTILS_HPP
