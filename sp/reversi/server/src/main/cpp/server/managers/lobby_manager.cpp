#include "lobby_manager.hpp"

std::shared_ptr<lobby> lobby_manager::create_lobby(const std::string &name, const std::shared_ptr<client> &creator) {
    std::lock_guard lock_guard(*this->mutex);
    if (is_name_taken_unsafe(name)){
        return nullptr;
    }

    auto new_lobby = std::make_shared<lobby>(name, creator);
    (*this->_lobbies)[name] = new_lobby;
    creator->set_lobby(new_lobby);
    return new_lobby;
}

bool lobby_manager::is_name_taken_unsafe(const std::string &name) {
    return this->_lobbies->contains(name);
}

void lobby_manager::exit_lobby(const std::shared_ptr<client> &player) {
    auto lobby = player->get_lobby();
    if (lobby == nullptr){
        return;
    }

}
