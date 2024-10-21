
#include "lobby.hpp"
#include "fmt/format.h"

#include <utility>

lobby::lobby(std::string name, const std::shared_ptr<client> &host) : _name(std::move(name)),
                                                                      _first_player(host) {

}

bool lobby::remove_player(const std::shared_ptr<client> &player) {
    std::unique_lock<std::shared_mutex> unique_lock(*this->shared_mutex);
    if (player != nullptr) {
        if (player == _first_player) {
            this->_first_player = this->_second_player;
            this->_second_player = nullptr;
        } else if (player == _second_player) {
            this->_second_player = nullptr;
        }
    }
    return _first_player != nullptr;
}

void lobby::clear_lobby() {
    std::unique_lock<std::shared_mutex> unique_lock(*this->shared_mutex);
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
    std::unique_lock<std::shared_mutex> unique_lock(*this->shared_mutex);
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
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    return this->_name;
}

std::shared_ptr<client> lobby::get_host() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    return this->_first_player;
}

bool lobby::is_available() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    return is_available_unsafe();
}

bool lobby::is_started() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    return is_started_unsafe();
}

bool lobby::connect_player(const std::shared_ptr<client>& player, const std::shared_ptr<lobby>& lobby_to_connect) {
    std::unique_lock<std::shared_mutex> unique_lock(*lobby_to_connect->shared_mutex);
    if (!lobby_to_connect->is_available_unsafe()){
        return false;
    }
    lobby_to_connect->_second_player = player;
    player->set_lobby(lobby_to_connect);
    return true;
}

std::shared_ptr<std::vector<std::shared_ptr<client>>> lobby::get_players() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    auto vector = std::make_shared<std::vector<std::shared_ptr<client>>>();
    vector->push_back(this->_first_player);
    vector->push_back(this->_second_player);
    return vector;
}

bool lobby::is_available_unsafe() const {
    return this->_first_player != nullptr && this->_second_player == nullptr && !is_started_unsafe();
}

bool lobby::is_started_unsafe() const {
    return this->_game != nullptr;
}

bool lobby::start_game() {
    std::unique_lock<std::shared_mutex> unique_lock(*this->shared_mutex);
    if (_first_player == nullptr || _second_player == nullptr){
        return false;
    }
    this->_game = std::make_shared<reversi_game>(_first_player, _second_player);
    return true;
}
