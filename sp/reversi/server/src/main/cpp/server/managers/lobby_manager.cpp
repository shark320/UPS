#include "lobby_manager.hpp"
#include "fmt/format.h"

static auto LOGGER = log4cxx::Logger::getLogger("lobby_manager");

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
    LOGGER->debug(fmt::format("Removing player {} from the lobby {}", player->to_string(), lobby->to_string()));
    if (!lobby->remove_player(player)){
        remove_lobby(lobby->get_name_unsafe());
    }
    player->set_lobby(nullptr);
}

void lobby_manager::remove_lobby(const std::string &name) {

    std::lock_guard lock_guard(*this->mutex);
    auto lobby_it = this->_lobbies->find(name);
    if (lobby_it == this->_lobbies->end()){
        return;
    }
    auto lobby = lobby_it->second;
    LOGGER->debug(fmt::format("Removing lobby {}", lobby->to_string()));
    lobby->clear_lobby();
    this->_lobbies->erase(lobby_it);
}
