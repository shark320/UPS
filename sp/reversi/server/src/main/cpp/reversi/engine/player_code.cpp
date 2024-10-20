#include "player_code.hpp"

player_code player_code_mapper::get_player_code(int id) {
    //TODO: implement or remove
    return player_code::NO_PLAYER;
}

std::string player_code_mapper::get_string(player_code player_code) {
    switch(player_code){
        case player_code::WHITE_PLAYER: return "WHITE_PLAYER";
        case player_code::BLACK_PLAYER: return "BLACK_PLAYER";
        case player_code::NO_PLAYER: return "NO_PLAYER";
    }
}
