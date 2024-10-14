#pragma once

#include <memory>
#include <unordered_map>
#include "../../game/lobby/lobby.hpp"

using lobbies_map_t = std::unordered_map<std::string, std::shared_ptr<lobby>>;

class lobby_manager {
private:
    std::shared_ptr<lobbies_map_t> _lobbies = std::make_shared<lobbies_map_t>();

    bool is_name_taken_unsafe(const std::string& name);

public:
    std::shared_ptr<std::mutex> mutex = std::make_shared<std::mutex>();

    std::shared_ptr<lobby> create_lobby(const std::string& name, const std::shared_ptr<client>& creator);
};
