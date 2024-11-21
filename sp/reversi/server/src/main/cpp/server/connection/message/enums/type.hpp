#pragma once

#include <map>
#include <unordered_map>

enum class type {
    GET = 1,
    POST = 2,
    NULL_TYPE = 0
};

class type_mapper {
private:
    static const std::unordered_map<int, type> type_map;

public:
    static type get_type(int id);

    static int get_id(type _type);
};


enum class subtype {
    NULL_SUBTYPE = 0,
    PING = 1,
    LOGIN = 2,
    CREATE_GAME = 3,
    LOBBY_EXIT = 4,
    LOGOUT = 5,
    START_GAME = 6,
    LOBBIES_LIST = 7,
    LOBBY_CONNECT = 8,
    GAME_STATE = 9,
    GAME_MOVE = 10,
    LOBBY_STATE = 11,
    HANDSHAKE = 12
};

class subtype_mapper {


private:
    static const std::unordered_map<int, subtype> subtype_map;

public:
    static subtype get_subtype(int id);

    static int get_id(subtype _subtype);
};