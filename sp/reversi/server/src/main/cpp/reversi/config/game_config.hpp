#pragma once

#include <string>
#include <memory>
#include "../board/board.hpp"
#include "SimpleIni.h"


class game_config {
private:


public:
    game_config(b_size bw, b_size bh, b_size ix, b_size iy);

    game_config(const std::shared_ptr<CSimpleIniA>& ini_config);

    game_config();

    const b_size board_width;
    const b_size board_height;
    const b_size init_x;
    const b_size init_y;
};

