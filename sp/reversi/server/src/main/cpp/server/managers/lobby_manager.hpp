#pragma once

#include <memory>
#include <unordered_map>
#include "../../game/lobby/lobby.hpp"

using lobbies_map_t = std::unordered_map<std::string, std::shared_ptr<lobby>>;
using hosts_map_t = std::unordered_map<std::shared_ptr<lobby>, std::shared_ptr<client>>;


class lobby_manager {
private:
    std::shared_ptr<lobbies_map_t> _lobbies = std::make_shared<lobbies_map_t>();
    std::shared_ptr<hosts_map_t> _hosts = std::make_shared<hosts_map_t>();

    bool is_name_taken_unsafe(const std::string& name);

    void remove_lobby_unsafe(const std::string& name);

public:
    std::shared_ptr<std::shared_mutex> shared_mutex = std::make_shared<std::shared_mutex>();

    [[nodiscard]] std::shared_ptr<lobby> create_lobby(const std::string& name, const std::shared_ptr<client>& host);

    void exit_lobby(const std::shared_ptr<client>& player);

    [[nodiscard]] std::shared_ptr<hosts_map_t> get_hosts() const;

    [[nodiscard]] std::shared_ptr<lobby> get_lobby(const std::string& name) const;

    [[nodiscard]] std::shared_ptr<std::unordered_map<std::string, std::string>> get_available_lobby_names_and_hosts() const;
};
