#include "reversi_game.hpp"


reversi_game::reversi_game(const std::shared_ptr<client> &white_player_client, const std::shared_ptr<client> &black_player_client){
    this->engine = std::make_shared<reversi_engine>();
    //TODO: random player color distribution
    this->_white_player = std::make_shared<player>(white_player_client, player_code::WHITE_PLAYER);
    this->_black_player = std::make_shared<player>(black_player_client, player_code::BLACK_PLAYER);
    this->_current_player = this->_white_player;
    this->engine->create_board(DEFAULT_BOARD_WIDTH, DEFAULT_BOARD_HEIGHT, DEFAULT_INIT_X, DEFAULT_INIT_Y);
}

std::shared_ptr<player> reversi_game::get_client_player(const std::shared_ptr<client>& client) const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    if (this->_white_player != nullptr && this->_white_player->get_client() == client){
        return this->_white_player;
    }
    if (this->_black_player != nullptr && this->_black_player->get_client() == client){
        return this->_black_player;
    }
    return nullptr;
}

std::shared_ptr<player> reversi_game::get_current_player() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    return this->_current_player;
}

std::string reversi_game::get_board_representation() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    const auto board = this->engine->get_board();
    const auto board_size = board->get_rows() * board->get_cols();
    std::string board_str = std::string(board_size, '\0');
    const auto cells = board->get_cells();
    for (size_t i = 0; i < board_size; ++i){
        board_str.insert(i, std::to_string(static_cast<int>(cells.at(i))));
    }
    return board_str;
}
