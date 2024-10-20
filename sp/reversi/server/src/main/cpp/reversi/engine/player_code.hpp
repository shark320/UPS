#pragma once

#include <unordered_map>
#include <string>

enum class player_code {
    WHITE_PLAYER = 1,
    BLACK_PLAYER = -1,
    NO_PLAYER = 0
};

class player_code_mapper {

private:
    static const std::unordered_map<int, player_code> player_code_map;

public:
    static player_code get_player_code(int id);

    static std::string get_string(player_code player_code);
};

