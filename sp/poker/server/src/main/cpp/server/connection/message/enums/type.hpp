#pragma once

#include <map>
#include <unordered_map>

enum class type {
    GET = 1,
    POST = 2,
    NULL_TYPE = -1
};

class type_mapper {
private:
    static const std::unordered_map<int, type> type_map;

public:
    static type get_type(int id);

    static int get_id(type _type);
};


enum class subtype {
    PING = 1,
    NULL_SUBTYPE = -1
};

class subtype_mapper {


private:
    static const std::unordered_map<int, subtype> subtype_map;

public:
    static subtype get_subtype(int id);

    static int get_id(subtype _subtype);
};