#include "game_engine.hpp"

void game_engine::initialize_board(b_size init_x, b_size init_y) {
    if (game_board->get_cols() <= (init_x + 1) || game_board->get_rows() <= (init_y + 1)) {
        //TODO: exception throwing on invalid init
        return;
    }
    game_board->set_at(init_x, init_y, WHITE_PLAYER);
    game_board->set_at(init_x + 1, init_y, BLACK_PLAYER);
    game_board->set_at(init_x, init_y + 1, BLACK_PLAYER);
    game_board->set_at(init_x + 1, init_y + 1, WHITE_PLAYER);
}

size_t game_engine::get_possible_moves_count(player_code player) {
    size_t moves_count = 0;
    auto available_moves = _get_possible_moves(moves_count, player);
    return moves_count;
}

std::vector<bool> game_engine::_get_possible_moves(size_t &moves_count, player_code player) {
    size_t x;
    size_t y;
    auto moves = std::vector<bool>(game_board->get_cols() * game_board->get_cols());
    moves_count = 0;
    for (x = 0; x < this->game_board->get_cols(); ++x) {
        for (y = 0; y < this->game_board->get_rows(); ++y) {
            if (_is_valid_move(x, y, player)) {
                moves[y * this->game_board->get_cols() + x] = true;
                ++moves_count;
            }
        }
    }
    return moves;
}

player_code game_engine::_get_opponent_code(player_code player) {
    return (player == BLACK_PLAYER) ? WHITE_PLAYER : BLACK_PLAYER;
}

size_t game_engine::_is_valid_direction(b_size x, b_size y, int dir_x, int dir_y, player_code player) {
    player_code opponent = _get_opponent_code(player);
    size_t steps = 1;
    b_size pos_x;
    b_size pos_y;
    player_code cell;
    /*Check if move is in bounds and an opponent cell*/
    if ((x + dir_x >= this->game_board->get_cols()) ||
        (y + dir_y >= this->game_board->get_rows() || this->game_board->get_at(x + dir_x, y + dir_y) != opponent)) {
        return 0;
    }
    pos_x = x + steps * dir_x;
    pos_y = y + steps * dir_y;
    /*Check specified direction for a valid sequence of opponent cells*/
    while ((pos_x < this->game_board->get_cols()) && (pos_y < this->game_board->get_rows())) {
        cell = this->game_board->get_at(pos_x, pos_y);
        /*Empty cell -> move is invalid*/
        if (cell == NO_PLAYER) {
            return 0;
        }
        /*Player cell -> return number of steps (cells) in between*/
        if (cell == player) {
            return steps;
        }
        steps++;
        /*Calculate next cell for check*/
        pos_x = x + steps * dir_x;
        pos_y = y + steps * dir_y;
    }
    return 0;
}

bool game_engine::_is_valid_move(b_size x, b_size y, player_code player) {
    int dir_x;
    int dir_y;
    if (x >= this->game_board->get_cols() || y >= this->game_board->get_rows()) {
        return false;
    }
    if (this->game_board->get_at(x, y) != NO_PLAYER) {
        return false;
    }
    /*Check throw all possible directions*/
    for (dir_x = -1; dir_x <= 1; ++dir_x) {
        for (dir_y = -1; dir_y <= 1; ++dir_y) {
            if ((dir_x != 0 || dir_y != 0) && _is_valid_direction(x, y, dir_x, dir_y, player)) {
                return true;
            }
        }
    }
    return false;
}

bool game_engine::no_available_moves() {
    return (get_possible_moves_count(BLACK_PLAYER) == 0) && (get_possible_moves_count(WHITE_PLAYER) == 0);
}

int game_engine::count_players_scores(b_size &bp_scores, b_size &wp_scores) {
    size_t x;
    size_t y;
    char c;
    bp_scores = 0;
    wp_scores = 0;

    for (y = 0; y < this->game_board->get_rows(); ++y) {
        for (x = 0; x < this->game_board->get_cols(); ++x) {
            c = this->game_board->get_at(x, y);
            if (c == BLACK_PLAYER) {
                ++bp_scores;
            } else if (c == WHITE_PLAYER) {
                ++wp_scores;
            }
        }
    }
    return 0;
}

size_t game_engine::_make_move(size_t x, size_t y, char player) {
    int dir_x;
    int dir_y;
    size_t i;
    size_t steps;
    size_t count = 0;
    this->game_board->set_at(x, y, player);
    for (dir_x = -1; dir_x <= 1; ++dir_x) {
        for (dir_y = -1; dir_y <= 1; ++dir_y) {
            steps = _is_valid_direction(x, y, dir_x, dir_y, player);
            count += steps;
            for (i = 1; i <= steps; ++i) {
                this->game_board->set_at(x + i * dir_x, y + i * dir_y, player);
            }
        }
    }
    return count;
}

bool game_engine::process_move(b_size x, b_size y, player_code player) {
    if (!_is_valid_move(x, y, player)) {
        return false;
    }
    _make_move( x, y, player);
    return true;
}



