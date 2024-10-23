
#include "game_config.hpp"

/**
 * @details Default game board width (columns)
 */
const b_size DEFAULT_BOARD_WIDTH_CONFIG = 8;

/**
 * @details Default game board height (rows)
 */
const b_size DEFAULT_BOARD_HEIGHT_CONFIG = 8;

/**
 * @details Default start stones X position coordinates - column (Starts from 0)
 */
const b_size DEFAULT_INIT_X_CONFIG = 3;

/**
 * @details Default start stones Y position coordinates - row (Starts from 0)
 */
const b_size DEFAULT_INIT_Y_CONFIG = 3;

game_config::game_config(b_size bw, b_size bh, b_size ix, b_size iy)
        : board_width(bw), board_height(bh), init_x(ix), init_y(iy) {}

game_config::game_config(): game_config(DEFAULT_BOARD_WIDTH_CONFIG, DEFAULT_BOARD_HEIGHT_CONFIG,DEFAULT_INIT_X_CONFIG,DEFAULT_INIT_Y_CONFIG) {

}

b_size game_config::get_board_width() const {
    return board_width;
}

b_size game_config::get_board_height() const {
    return board_height;
}

void game_config::set_board_width(b_size bw) {
    board_width = bw;
}

void game_config::set_board_height(b_size bh) {
    board_height = bh;
}

b_size game_config::get_init_x() const {
    return init_x;
}

void game_config::set_init_x(b_size ix) {
    init_x = ix;
}

b_size game_config::get_init_y() const {
    return init_y;
}

void game_config::set_init_y(b_size iy) {
    init_y = iy;
}



