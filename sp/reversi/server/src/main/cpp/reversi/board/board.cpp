#include "board.hpp"


board::board(b_size rows, b_size cols) : rows(rows), cols(cols), cells(rows * cols, player_code::NO_PLAYER) {

}


board::board(const board &other) : board(other.rows, other.cols) {
}


player_code board::get_at(b_size x, b_size y) {
    return cells[y * cols + x];
}

void board::set_at(b_size x, b_size y, player_code val) {
    //TODO: Throwing exception on invalid coordinates
    cells[y * cols + x] = val;
}

b_size board::get_rows() const {
    return this->rows;
}

b_size board::get_cols() const {
    return this->cols;
}

std::vector<player_code> board::get_cells() const {
    return this->cells;
}

//
//std::string board::print() {
//    b_size i,j,row_digits;
//    b_val c;
//    row_digits = count_digits(this->rows);
//}
