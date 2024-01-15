#include "status.hpp"

const std::map<int, status::_enum> status::status_map = {
        {200, status::_enum::OK}
};


status::_enum status::get_status(int id) {
    auto it = status_map.find(id);
    if (it != status_map.end()) {
        return it->second;
    }
    return _enum::NULL_STATUS; // Return a default _value when the ID is not found.
}

int status::get_id(status::_enum _status) {
    for (const auto& entry : status_map) {
        if (entry.second == _status) {
            return entry.first;
        }
    }
    return 0;  // Handle cases where the status ID is not found
}
