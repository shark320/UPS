#pragma once

#include <unordered_map>

enum class status{
    NULL_STATUS = 000,
    OK = 200,
    RESET = 205,
    MOVED_PERMANENTLY = 301,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    NOT_FOUND = 404,
    CONFLICT = 409,
    NOT_ALLOWED = 405,

};

class status_mapper {

private:
    static const std::unordered_map<int, status> status_map;

public:
    static status get_status(int id);

    static int get_id(status _status);
};


