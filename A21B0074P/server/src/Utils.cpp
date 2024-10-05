#include "Utils.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "Constants.hpp"


using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::stringstream;
using std::setw;
using std::setfill;

void log(const string& message) {
    if (IS_DEBUG) {
        cout << message << endl;
    }
}

string trimEndOfLine(const string& str) {
    string trimmed = str;
    while (!trimmed.empty() && (trimmed.back() == '\r' || trimmed.back() == '\n')) {
        trimmed.pop_back();
    }
    return trimmed;
}

string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool containsOnlyControlChars(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](char c) {
        return c == '\n' || c == '\r' || c == '\t';
    });
}

bool areAllComponentsValid(const std::vector<std::string>& components) {
    for (const auto& component : components) {
        if (component.empty()) {
            return false;
        }
    }
    return true;
}

bool containsOnlyControlChars(const char* str) {
    if (str == nullptr) return false; // Checking for null

    const char* ptr = str;
    while (*ptr != '\0') {
        if (*ptr != '\n' && *ptr != '\r' && *ptr != '\t') {
            return false; // Found the symbol, that not matches
        }
        ++ptr;
    }
    return true; // All symbols match
}

bool trimCompare(const string& lhs, const string& rhs) {
    string trimmed_lhs = trimEndOfLine(lhs);
    string trimmed_rhs = trimEndOfLine(rhs);
    return trimmed_lhs.compare(trimmed_rhs) == 0;
}

vector<string> splitMessage(const string& message, char delimiter) {
    vector<string> components;
    stringstream ss(message);
    string part;
    while (getline(ss, part, delimiter)) {
        components.push_back(part);
    }
    return components;
}

string generateHeader(int length) {
    stringstream ss;
    ss << setw(4) << setfill('0') << length;
    return ss.str();
}

string generateResponse(const string& opcode, bool status, const string& message) {
    stringstream response;
    response << VALID_HEADER << "1" << opcode[1];
    string finalMessage = (status ? "1;" : "0;" ) + message;
    string header = generateHeader(finalMessage.size());
    response << header << finalMessage + "\n";
    return response.str();
}

string generatePositiveResponse(const string& opcode, const string& message) {
    return generateResponse(opcode, true, message);
}

string generateNegativeResponse(const string& opcode, const string& message) {
    return generateResponse(opcode, false, message);
}

bool isBinaryString(const string& str) {
    for (char c : str) {
        if (c != '0' && c != '1') {
            return false;
        }
    }
    return true;
}