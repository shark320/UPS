#pragma once

#include <map>

class type {
public:
    enum class _enum {
        GET = 1,
        POST = 2
    };

private:
    static const std::map<int, type::_enum> type_map;

public:
    static _enum get_type(int id);

    static int get_id(_enum type);
};


class subtype {
public:
    enum class _enum {
        PING = 1
    };

private:
    static const std::map<int, subtype::_enum> subtype_map;

public:
    static _enum get_subtype(int id);

    static int get_id(_enum subtype);
};