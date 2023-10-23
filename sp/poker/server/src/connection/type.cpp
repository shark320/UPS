#pragma once

#include <map>

class Type {
public:
    enum Enum {
        GET = 1,
        POST = 2
    };

private:
    static const std::map<int, Type::Enum> typeMap;

public:
    static Enum getType(int id) {
        auto it = typeMap.find(id);
        if (it != typeMap.end()) {
            return it->second;
        }
        return GET; // Return a default value when the ID is not found.
    }

    static int getId(Enum type) {
        for (const auto& entry : typeMap) {
            if (entry.second == type) {
                return entry.first;
            }
        }
        return -1;  // Handle cases where the type ID is not found
    }
};

const std::map<int, Type::Enum> Type::typeMap = {
        {1, Type::GET},
        {2, Type::POST}
};

