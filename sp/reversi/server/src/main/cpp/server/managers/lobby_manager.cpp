#include "lobby_manager.hpp"
#include "fmt/format.h"

static auto LOGGER = log4cxx::Logger::getLogger("lobby_manager");

std::shared_ptr<lobby> lobby_manager::create_lobby(const std::string &name, const std::shared_ptr<client> &host) {
    std::unique_lock<std::shared_mutex> unique_lock(*this->shared_mutex);
    if (is_name_taken_unsafe(name)) {
        return nullptr;
    }

    auto new_lobby = std::make_shared<lobby>(name, host);
    (*this->_lobbies)[name] = new_lobby;
    host->set_lobby(new_lobby);
    (*this->_hosts)[new_lobby] = host;
    return new_lobby;
}

bool lobby_manager::is_name_taken_unsafe(const std::string &name) {
    return this->_lobbies->contains(name);
}

void lobby_manager::exit_lobby(const std::shared_ptr<client> &player) {
    auto lobby = player->get_lobby();
    if (lobby == nullptr) {
        return;
    }
    LOGGER->debug(fmt::format("Removing player {} from the lobby {}", player->to_string(), lobby->to_string()));
    if (!lobby->remove_player(player)) {
        remove_lobby(lobby->get_name_unsafe());
    }
    (*this->_hosts)[lobby] = lobby->get_host();

    player->set_lobby(nullptr);
}

void lobby_manager::remove_lobby(const std::string &name) {
    std::unique_lock<std::shared_mutex> unique_lock(*this->shared_mutex);
    auto lobby_it = this->_lobbies->find(name);
    if (lobby_it == this->_lobbies->end()) {
        return;
    }
    auto lobby = lobby_it->second;
    LOGGER->debug(fmt::format("Removing lobby {}", lobby->to_string()));
    lobby->clear_lobby();
    this->_lobbies->erase(lobby_it);
    this->_hosts->erase(lobby);
}

std::shared_ptr<hosts_map_t> lobby_manager::get_hosts() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    return this->_hosts;
}

std::shared_ptr<std::unordered_map<std::string, std::string>> lobby_manager::get_lobby_names_and_hosts() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    auto map = std::make_shared<std::unordered_map<std::string, std::string>>();
    for (const auto& host_pair: *this->_hosts){
        (*map)[host_pair.first->get_name()] = host_pair.second->get_username();
    }
    return map;
}



