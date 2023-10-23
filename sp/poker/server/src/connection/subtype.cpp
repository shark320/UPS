#pragma once
#include <map>

class Subtype {
public:
    enum Enum {
        PING = 1
    };

private:
    static const std::map<int, Subtype::Enum> subtypeMap;

public:
    static Enum getSubtype(int id) {
        auto it = subtypeMap.find(id);
        if (it != subtypeMap.end()) {
            return it->second;
        }
        return PING; // Return a default value when the ID is not found.
    }

    static int getId(Enum subtype) {
        for (const auto& entry : subtypeMap) {
            if (entry.second == subtype) {
                return entry.first;
            }
        }
        return -1;  // Handle cases where the subtype ID is not found
    }
};

const std::map<int, Subtype::Enum> Subtype::subtypeMap = {
        {1, Subtype::PING}
};

