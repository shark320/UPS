
#include "lobby.hpp"
#include "fmt/format.h"

#include <utility>

lobby::lobby(std::string name, const std::shared_ptr<client> &creator) : _name(std::move(name)),
                                                                         _first_player(creator) {

}

bool lobby::remove_player(const std::shared_ptr<client> &player) {
    std::lock_guard<std::mutex> lock_guard(*this->mutex);
    if (player != nullptr) {
        if (player == _first_player) {
            this->_first_player = this->_second_player;
        } else if (player == _second_player) {
            this->_second_player = nullptr;
        }
    }
    return _first_player != nullptr || _second_player != nullptr;
}

void lobby::clear_lobby() {
    std::lock_guard<std::mutex> lock_guard(*this->mutex);
    if (this->_first_player != nullptr) {
        this->_first_player->clear_lobby();
        this->_first_player = nullptr;
    }
    if (this->_second_player != nullptr) {
        this->_second_player->clear_lobby();
        this->_second_player = nullptr;
    }
}

std::string lobby::get_name_unsafe() const {
    return this->_name;
}

std::string lobby::to_string() const {
    std::lock_guard<std::mutex> lock_guard(*this->mutex);
    std::string first_player_str = this->_first_player == nullptr ? "null" : this->_first_player->to_string();
    std::string second_player_str = this->_second_player == nullptr ? "null" : this->_second_player->to_string();

    return fmt::format(
            "lobby(name='{}', first_player={}, second_player={})",
            this->_name,
            first_player_str,
            second_player_str
    );
}

std::string lobby::get_name() const {
    std::lock_guard<std::mutex> lock_guard(*this->mutex);
    return this->_name;
}
