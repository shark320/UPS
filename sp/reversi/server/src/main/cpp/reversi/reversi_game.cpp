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
    return get_client_player_unsafe(client);
}

std::shared_ptr<player> reversi_game::get_client_player_unsafe(const std::shared_ptr<client> &client) const {
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

//std::string reversi_game::get_board_representation() const {
//    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
//    const auto board = this->_engine->get_board();
//    const auto board_size = board->get_rows() * board->get_cols();
//    std::string board_str = std::string(board_size, '\0');
//    const auto cells = board->get_cells();
//    for (size_t i = 0; i < board_size; ++i) {
//        board_str.insert(i, std::to_string(static_cast<int>(cells.at(i))));
//    }
//    return board_str;
//}

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
    return get_opponent_unsafe(player);
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
    if (player != this->_current_player) {
        return INVALID_PLAYER;
    }
    bool result = _engine->process_move(x, y, player->get_player_code());
    if (result) {
        this->_current_player = get_opponent_unsafe(player);
        this->_last_move = std::make_shared<move_coordinates>(x, y);
        if (_engine->get_possible_moves_count(this->_current_player->get_player_code()) <= 0){
            auto player_scores = this->_engine->count_players_scores();
            this->_winner = player_scores->white_player > player_scores->black_player ? this->_white_player : this->_black_player;
        }
    }
    return result ? SUCCESS : INVALID_COORDINATES;
}

move_result reversi_game::process_move(b_size x, b_size y, const std::shared_ptr<client> &client) {
    std::unique_lock<std::shared_mutex> unique_lock(*this->_shared_mutex);
    const auto client_player = get_client_player_unsafe(client);
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

bool reversi_game::is_game_over() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
    return get_winner() != nullptr;
}

std::shared_ptr<player> reversi_game::get_opponent_unsafe(const std::shared_ptr<player> &player) const {
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

std::shared_ptr<std::vector<int>> reversi_game::get_board_representation() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->_shared_mutex);
    const auto board_vector = std::make_shared<std::vector<int>>();
    const auto board = this->_engine->get_board();
    for (const auto cell: board->get_cells()){
        board_vector->push_back(static_cast<const int>(cell));
    }
    return board_vector;
}



