#include "type.hpp"

const std::map<int, type::_enum> type::type_map = {
        {1, type::_enum::GET},
        {2, type::_enum::POST}
};

 type::_enum type::get_type(int id) {
    auto it = type_map.find(id);
    if (it != type_map.end()) {
        return it->second;
    }
    return _enum::GET; // Return a default _value when the ID is not found.
}

int type::get_id(_enum type) {
    for (const auto& entry : type_map) {
        if (entry.second == type) {
            return entry.first;
        }
    }
    return -1;  // Handle cases where the type ID is not found
}


const std::map<int, subtype::_enum> subtype::subtype_map = {
        {1, subtype::_enum::PING}
};


subtype::_enum subtype::get_subtype(int id) {
    auto it = subtype_map.find(id);
    if (it != subtype_map.end()) {
        return it->second;
    }
    return _enum::PING; // Return a default _value when the ID is not found.
}

int subtype::get_id(_enum subtype) {
    for (const auto& entry : subtype_map) {
        if (entry.second == subtype) {
            return entry.first;
        }
    }
    return -1;  // Handle cases where the subtype ID is not found
}