#include "reversi_engine.hpp"

void reversi_engine::initialize_board(b_size init_x, b_size init_y) {
    if (_game_board == nullptr){
        //TODO: throw exception on nullptr
        return;
    }
    if (_game_board->get_cols() <= (init_x + 1) || _game_board->get_rows() <= (init_y + 1)) {
        //TODO: exception throwing on invalid init
        return;
    }
    _game_board->set_at(init_x, init_y, player_code::WHITE_PLAYER);
    _game_board->set_at(init_x + 1, init_y, player_code::BLACK_PLAYER);
    _game_board->set_at(init_x, init_y + 1, player_code::BLACK_PLAYER);
    _game_board->set_at(init_x + 1, init_y + 1, player_code::WHITE_PLAYER);
}

size_t reversi_engine::get_possible_moves_count(player_code player) {
    size_t moves_count = 0;
    auto available_moves = _get_possible_moves(moves_count, player);
    return moves_count;
}

std::vector<bool> reversi_engine::_get_possible_moves(size_t &moves_count, player_code player) {
    size_t x;
    size_t y;
    auto moves = std::vector<bool>(_game_board->get_cols() * _game_board->get_cols());
    moves_count = 0;
    for (x = 0; x < this->_game_board->get_cols(); ++x) {
        for (y = 0; y < this->_game_board->get_rows(); ++y) {
            if (_is_valid_move(x, y, player)) {
                moves[y * this->_game_board->get_cols() + x] = true;
                ++moves_count;
            }
        }
    }
    return moves;
}

player_code reversi_engine::_get_opponent_code(player_code player) {
    return (player == player_code::BLACK_PLAYER) ? player_code::WHITE_PLAYER : player_code::BLACK_PLAYER;
}

size_t reversi_engine::_is_valid_direction(b_size x, b_size y, int dir_x, int dir_y, player_code player) {
    player_code opponent = _get_opponent_code(player);
    size_t steps = 1;
    b_size pos_x;
    b_size pos_y;
    player_code cell;
    /*Check if move is in bounds and an opponent cell*/
    if ((x + dir_x >= this->_game_board->get_cols()) ||
        (y + dir_y >= this->_game_board->get_rows() || this->_game_board->get_at(x + dir_x, y + dir_y) != opponent)) {
        return 0;
    }
    pos_x = x + steps * dir_x;
    pos_y = y + steps * dir_y;
    /*Check specified direction for a valid sequence of opponent cells*/
    while ((pos_x < this->_game_board->get_cols()) && (pos_y < this->_game_board->get_rows())) {
        cell = this->_game_board->get_at(pos_x, pos_y);
        /*Empty cell -> move is invalid*/
        if (cell == player_code::NO_PLAYER) {
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

bool reversi_engine::_is_valid_move(b_size x, b_size y, player_code player) {
    int dir_x;
    int dir_y;
    if (x >= this->_game_board->get_cols() || y >= this->_game_board->get_rows()) {
        return false;
    }
    if (this->_game_board->get_at(x, y) != player_code::NO_PLAYER) {
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

bool reversi_engine::no_available_moves() {
    return (get_possible_moves_count(player_code::BLACK_PLAYER) == 0) && (get_possible_moves_count(player_code::WHITE_PLAYER) == 0);
}

int reversi_engine::count_players_scores(b_size &bp_scores, b_size &wp_scores) {
    size_t x;
    size_t y;
    player_code c;
    bp_scores = 0;
    wp_scores = 0;

    for (y = 0; y < this->_game_board->get_rows(); ++y) {
        for (x = 0; x < this->_game_board->get_cols(); ++x) {
            c = this->_game_board->get_at(x, y);
            if (c == player_code::BLACK_PLAYER) {
                ++bp_scores;
            } else if (c == player_code::WHITE_PLAYER) {
                ++wp_scores;
            }
        }
    }
    return 0;
}

size_t reversi_engine::_make_move(size_t x, size_t y, player_code player) {
    int dir_x;
    int dir_y;
    size_t i;
    size_t steps;
    size_t count = 0;
    this->_game_board->set_at(x, y, player);
    for (dir_x = -1; dir_x <= 1; ++dir_x) {
        for (dir_y = -1; dir_y <= 1; ++dir_y) {
            steps = _is_valid_direction(x, y, dir_x, dir_y, player);
            count += steps;
            for (i = 1; i <= steps; ++i) {
                this->_game_board->set_at(x + i * dir_x, y + i * dir_y, player);
            }
        }
    }
    return count;
}

bool reversi_engine::process_move(b_size x, b_size y, player_code player) {
    if (!_is_valid_move(x, y, player)) {
        return false;
    }
    _make_move( x, y, player);
    return true;
}

void reversi_engine::create_board(b_size bw, b_size bh, b_size ix, b_size iy) {
    this->_game_board = std::make_shared<board>(bw, bh);
    initialize_board(ix,iy);
}

std::shared_ptr<board> reversi_engine::get_board() const {
    return this->_game_board;
}



