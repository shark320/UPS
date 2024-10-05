#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include "../consts/game_consts.hpp"

using b_size = size_t;

class board{
private:
    std::vector<player_code>cells;
    b_size rows;
    b_size cols;

public:
    board(b_size rows, b_size cols);

    board(const board& other);

    player_code get_at(b_size x, b_size y);

    void set_at(b_size x, b_size y, player_code val);

    b_size get_rows();

    b_size get_cols();

//    std::string print();
};