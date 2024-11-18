#pragma once

#include "engine/reversi_engine.hpp"
#include "../server/connection/client/client.hpp"
#include "../game/player/player.hpp"
#include "config/game_config.hpp"

class player;

enum move_result {
    SUCCESS,
    INVALID_PLAYER,
    INVALID_COORDINATES,
    NO_PLAYER,
    GAME_OVER,
};

class move_coordinates {
public:
    const b_size x;
    const b_size y;

    move_coordinates(b_size x, b_size y) : x(x), y(y) {}
};

class reversi_game {
private:
    std::shared_ptr<reversi_engine> _engine;
    std::shared_ptr<player> _white_player;
    std::shared_ptr<player> _black_player;
    std::shared_ptr<player> _current_player;
    std::shared_ptr<player> _winner = nullptr;
    std::shared_ptr<move_coordinates> _last_move = std::make_shared<move_coordinates>(DEFAULT_INIT_X, DEFAULT_INIT_Y);

    std::shared_ptr<std::shared_mutex> _shared_mutex = std::make_shared<std::shared_mutex>();

    [[nodiscard]] move_result process_move_unsafe(b_size x, b_size y, const std::shared_ptr<player> &player);

    [[nodiscard]] std::shared_ptr<player> get_client_player_unsafe(const std::shared_ptr<client> &client) const;

    [[nodiscard]] std::shared_ptr<player> get_opponent_unsafe(const std::shared_ptr<player> &player) const;

public:
    reversi_game(const std::shared_ptr<client> &white_player_client,
                 const std::shared_ptr<client> &black_player_client,
                 const std::shared_ptr<game_config>& _game_config
                 );

    [[nodiscard]] std::shared_ptr<player> get_client_player(const std::shared_ptr<client> &client) const;

    [[nodiscard]] std::shared_ptr<player> get_current_player() const;

    [[nodiscard]] std::shared_ptr<client> get_current_player_client() const;

    [[nodiscard]] std::shared_ptr<client> get_opponent_client(const std::shared_ptr<client> &client) const;

    [[nodiscard]] std::shared_ptr<player> get_opponent(const std::shared_ptr<player> &player) const;

//    [[nodiscard]] std::string get_board_representation() const;

    [[nodiscard]] move_result process_move(b_size x, b_size y, const std::shared_ptr<client> &client);

    [[nodiscard]] std::shared_ptr<move_coordinates> get_last_move() const;

    [[nodiscard]] std::shared_ptr<player> get_winner() const;

    [[nodiscard]] std::shared_ptr<std::vector<int>> get_board_representation() const;

    [[nodiscard]] std::shared_ptr<board> get_board() const;

    [[nodiscard]] bool is_game_over() const;
};

