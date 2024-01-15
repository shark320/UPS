#pragma once

#include <map>

class status {
public:
    enum class _enum{
        NULL = 0,
        OK = 200
    };

private:
    static const std::map<int, status::_enum> status_map;

public:
    static _enum get_status(int id);

    static int get_id(_enum status);
};


