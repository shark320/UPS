#include "status.hpp"

const std::unordered_map<int, status> status_mapper::status_map = {
        {200, status::OK},
        {205, status::RESET},
        {0, status::NULL_STATUS},
        {400, status::BAD_REQUEST},
        {401, status::UNAUTHORIZED},
        {404, status::NOT_FOUND},
        {409, status::CONFLICT},
        {405, status::NOT_ALLOWED},
};


status status_mapper::get_status(int id) {
    auto it = status_map.find(id);
    if (it != status_map.end()) {
        return it->second;
    }
    return status::NULL_STATUS; // Return a default _value when the ID is not found.
}

int status_mapper::get_id(status _status) {
    for (const auto& entry : status_map) {
        if (entry.second == _status) {
            return entry.first;
        }
    }
    return -1;  // Handle cases where the status ID is not found
}
