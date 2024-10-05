
#include "game_config.hpp"

game_config::game_config(b_size bw, b_size bh, b_size ix, b_size iy)
        : board_width(bw), board_height(bh), init_x(ix), init_y(iy) {}

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

