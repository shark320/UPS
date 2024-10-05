#pragma once

#include <string>
#include "../board/board.hpp"


class game_config {
private:
    b_size board_width;
    b_size board_height;
    b_size init_x;
    b_size init_y;

public:
    game_config(b_size bw, b_size bh, b_size ix, b_size iy);

    [[nodiscard]] b_size get_board_width() const;

    void set_board_width(b_size bw);

    [[nodiscard]] b_size get_board_height() const;

    void set_board_height(b_size bh);

    [[nodiscard]] b_size get_init_x() const;

    void set_init_x(b_size ix);

    [[nodiscard]] b_size get_init_y() const;

    void set_init_y(b_size iy);
};

