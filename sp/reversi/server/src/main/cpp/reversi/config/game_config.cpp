
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

const static std::string GAME_SECTION = "Game";

const static std::string BOARD_WIDTH_KEY = "board_width";
const static std::string BOARD_HEIGHT_KEY = "board_height";
const static std::string BOARD_INIT_X_KEY = "init_x";
const static std::string BOARD_INIT_Y_KEY = "init_y";

game_config::game_config(b_size bw, b_size bh, b_size ix, b_size iy)
        : board_width(bw), board_height(bh), init_x(ix), init_y(iy) {}

game_config::game_config() : game_config(DEFAULT_BOARD_WIDTH_CONFIG, DEFAULT_BOARD_HEIGHT_CONFIG, DEFAULT_INIT_X_CONFIG,
                                         DEFAULT_INIT_Y_CONFIG) {

}

game_config::game_config(const std::shared_ptr<CSimpleIniA> &ini_config) : game_config(
        ini_config->GetLongValue(GAME_SECTION.c_str(), BOARD_WIDTH_KEY.c_str(), -1),
        ini_config->GetLongValue(GAME_SECTION.c_str(), BOARD_HEIGHT_KEY.c_str(), -1),
        ini_config->GetLongValue(GAME_SECTION.c_str(), BOARD_INIT_X_KEY.c_str(), -1),
        ini_config->GetLongValue(GAME_SECTION.c_str(), BOARD_INIT_Y_KEY.c_str(), -1)
) {
    long init_x_value = ini_config->GetLongValue(GAME_SECTION.c_str(), BOARD_INIT_X_KEY.c_str(), -1);
    long init_y_value = ini_config->GetLongValue(GAME_SECTION.c_str(), BOARD_INIT_Y_KEY.c_str(), -1);
    long board_width_value = ini_config->GetLongValue(GAME_SECTION.c_str(), BOARD_WIDTH_KEY.c_str(), -1);
    long board_height_value = ini_config->GetLongValue(GAME_SECTION.c_str(), BOARD_HEIGHT_KEY.c_str(), -1);

    if (board_width_value < 3 ||
        board_height_value < 3 ||
        (init_x_value < 0 || init_x_value >= board_width_value) ||
        (init_y_value < 0 || init_y_value >= board_height_value)
            ) {
        throw std::invalid_argument("Game configuration is invalid!");
    }
}



