#pragma once

#include "engine/reversi_engine.hpp"

class reversi_game {
private:
    std::shared_ptr<reversi_engine> engine;

public:
    reversi_game();

    void start_game();
};

