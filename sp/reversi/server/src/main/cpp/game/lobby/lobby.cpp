
#include "lobby.hpp"

#include <utility>

lobby::lobby(std::string  name, const std::shared_ptr<client>& creator): _name(std::move(name)), _first_player(creator) {

}

bool lobby::remove_player(const std::shared_ptr<client> &player) {
    if (player == nullptr){
        return _first_player != nullptr || _second_player != nullptr;
    }
    if (player == _first_player){

    }
}

bool lobby::reassign_lobby(const std::shared_ptr<client> &player) {
    if (_first_player == nullptr && _second_player == nullptr){
        return false;
    }
    if (_first_player != nullptr){
        return true;
    }
    this->_first_player = player;
    if (this->_second_player == player){
        this->_second_player = nullptr;
    }
    return true;
}
