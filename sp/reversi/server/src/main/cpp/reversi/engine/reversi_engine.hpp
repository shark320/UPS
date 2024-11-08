#pragma once

#include <memory>
#include "../board/board.hpp"
#include "../consts/game_consts.hpp"

class player_scores{
public:
    const int white_player;
    const int black_player;

    player_scores(int white_player, int black_player): white_player(white_player), black_player(black_player) {}
};

class reversi_engine {

private:
    std::shared_ptr<board> _game_board = nullptr;

public:
    void create_board(b_size bw, b_size bh, b_size ix, b_size iy);

    void initialize_board(b_size init_x, b_size init_y);

    bool no_available_moves();

    bool process_move(b_size x, b_size y, player_code player);

    size_t get_possible_moves_count(player_code player);

    std::shared_ptr<player_scores> count_players_scores();

    [[nodiscard]] std::shared_ptr<board> get_board() const;

private:

    bool _is_valid_move(b_size x, b_size y, player_code player);

    size_t _is_valid_direction(b_size x, b_size y, int dir_x, int dir_y, player_code player);

    player_code _get_opponent_code(player_code player);

    std::vector<bool> _get_possible_moves (size_t& moves_count, player_code player);

    size_t _make_move(size_t x, size_t y, player_code player);



};
