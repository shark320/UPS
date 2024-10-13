#include "type.hpp"

const std::unordered_map<int, type> type_mapper::type_map = {
        {1, type::GET},
        {2, type::POST},
        {-1, type::NULL_TYPE}
};

type type_mapper::get_type(int id) {
    auto it = type_map.find(id);
    if (it != type_map.end()) {
        return it->second;
    }
    return type::GET; // Return a default _value when the ID is not found.
}

int type_mapper::get_id(type _type) {
    for (const auto& entry : type_map) {
        if (entry.second == _type) {
            return entry.first;
        }
    }
    return -1;  // Handle cases where the type ID is not found
}


const std::unordered_map<int, subtype> subtype_mapper::subtype_map = {
        {1, subtype::PING},
        {2, subtype::LOGIN},
        {12, subtype::HANDSHAKE},
        {-1, subtype::NULL_SUBTYPE}
};


subtype subtype_mapper::get_subtype(int id) {
    auto it = subtype_map.find(id);
    if (it != subtype_map.end()) {
        return it->second;
    }
    return subtype::PING; // Return a default _value when the ID is not found.
}

int subtype_mapper::get_id(subtype subtype) {
    for (const auto& entry : subtype_map) {
        if (entry.second == subtype) {
            return entry.first;
        }
    }
    return -1;  // Handle cases where the subtype ID is not found
}