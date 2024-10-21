#pragma once

#include "engine/reversi_engine.hpp"
#include "../server/connection/client/client.hpp"
#include "../game/player/player.hpp"

class reversi_game {
private:
    std::shared_ptr<reversi_engine> engine;
    std::shared_ptr<player> _white_player;
    std::shared_ptr<player> _black_player;
    std::shared_ptr<player> _current_player;

    std::shared_ptr<std::shared_mutex> shared_mutex = std::make_shared<std::shared_mutex>();
public:
    reversi_game(const std::shared_ptr<client> &white_player_client,
                 const std::shared_ptr<client> &black_player_client);

    [[nodiscard]] std::shared_ptr<player> get_client_player(const std::shared_ptr<client> &client) const;

    [[nodiscard]] std::shared_ptr<player> get_current_player() const;

    [[nodiscard]] std::string get_board_representation() const;
};

