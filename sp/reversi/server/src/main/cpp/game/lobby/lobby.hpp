#pragma once

#include "../../server/connection/client/client.hpp"
#include "../../reversi/reversi_game.hpp"

class client;
class reversi_game;

class lobby {
private:
    std::shared_ptr<client> _first_player;
    std::shared_ptr<client> _second_player;
    std::shared_ptr<reversi_game> _game;
    std::string _name;

    const std::shared_ptr<std::shared_mutex> shared_mutex = std::make_shared<std::shared_mutex>();

public:
    lobby(std::string  name, const std::shared_ptr<client>& host);

    bool remove_player(const std::shared_ptr<client>& player);

    void clear_lobby();

    [[nodiscard]] std::string get_name_unsafe() const;

    [[nodiscard]] std::string get_name() const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] std::shared_ptr<client> get_host() const;

    [[nodiscard]] bool is_available() const;

    [[nodiscard]] bool is_available_unsafe() const;

    [[nodiscard]] bool is_started() const;

    [[nodiscard]] bool start_game();

    [[nodiscard]] bool is_started_unsafe() const;

    [[nodiscard]] static bool connect_player(const std::shared_ptr<client>& player, const std::shared_ptr<lobby>& lobby_to_connect);

    [[nodiscard]] std::shared_ptr<std::vector<std::shared_ptr<client>>>  get_players() const;

    [[nodiscard]] std::shared_ptr<reversi_game> get_game() const;
};
