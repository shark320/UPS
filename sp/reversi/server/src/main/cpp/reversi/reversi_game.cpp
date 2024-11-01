#include "reversi_game.hpp"


reversi_game::reversi_game(const std::shared_ptr<client> &white_player_client,
                           const std::shared_ptr<client> &black_player_client) {
    this->_engine = std::make_shared<reversi_engine>();
    //TODO: random player color distribution
    this->_white_player = std::make_shared<player>(white_player_client, player_code::WHITE_PLAYER);
    this->_black_player = std::make_shared<player>(black_player_client, player_code::BLACK_PLAYER);
    this->_current_player = this->_white_player;
    this->_engine->create_board(DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT, DEFAULT_INIT_X, DEFAULT_INIT_Y);
}

std::shared_ptr<player> reversi_game::get_client_player(const std::shared_ptr<client> &client) const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
    if (this->_white_player != nullptr && this->_white_player->get_client() == client) {
        return this->_white_player;
    }
    if (this->_black_player != nullptr && this->_black_player->get_client() == client) {
        return this->_black_player;
    }
    return nullptr;
}

std::shared_ptr<player> reversi_game::get_current_player() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
    return this->_current_player;
}

std::string reversi_game::get_board_representation() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
    const auto board = this->_engine->get_board();
    const auto board_size = board->get_rows() * board->get_cols();
    std::string board_str = std::string(board_size, '\0');
    const auto cells = board->get_cells();
    for (size_t i = 0; i < board_size; ++i) {
        board_str.insert(i, std::to_string(static_cast<int>(cells.at(i))));
    }
    return board_str;
}

std::shared_ptr<client> reversi_game::get_opponent_client(const std::shared_ptr<client> &client) const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
    const auto client_player = get_client_player(client);
    const auto opponent_player = get_opponent(client_player);
    if (opponent_player == nullptr) {
        return nullptr;
    }
    return opponent_player->get_client();
}

std::shared_ptr<player> reversi_game::get_opponent(const std::shared_ptr<player> &player) const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
    if (player == nullptr) {
        return nullptr;
    }
    switch (player->get_player_code()) {
        case player_code::BLACK_PLAYER:
            return this->_white_player;
        case player_code::WHITE_PLAYER:
            return this->_black_player;
        default:
            return nullptr;
    }
}

std::shared_ptr<client> reversi_game::get_current_player_client() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
    const auto current_player = get_current_player();
    if (current_player == nullptr) {
        return nullptr;
    }
    return current_player->get_client();
}

move_result reversi_game::process_move_unsafe(b_size x, b_size y, const std::shared_ptr<player> &player) {
    if (this->_winner != nullptr){
        return GAME_OVER;
    }
    if (player != get_current_player()) {
        return INVALID_PLAYER;
    }
    bool result = _engine->process_move(x, y, player->get_player_code());
    if (result) {
        this->_current_player = get_opponent(player);
        this->_last_move = std::make_shared<move_coordinates>(x, y);
    }
    return result ? SUCCESS : INVALID_COORDINATES;
}

move_result reversi_game::process_move(b_size x, b_size y, const std::shared_ptr<client> &client) {
    std::unique_lock<std::shared_mutex> unique_lock(*this->_shared_mutex);
    const auto client_player = get_client_player(client);
    if (client_player == nullptr) {
        return NO_PLAYER;
    }
    return process_move_unsafe(x, y, client_player);
}

std::shared_ptr<move_coordinates> reversi_game::get_last_move() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
    return this->_last_move;
}

std::shared_ptr<player> reversi_game::get_winner() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
    return this->_winner;
}

