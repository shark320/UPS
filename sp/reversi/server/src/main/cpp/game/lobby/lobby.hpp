#pragma once

#include "../../server/connection/client/client.hpp"
#include "../../reversi/reversi_game.hpp"

class lobby {
private:
    std::shared_ptr<client> _first_player;
    std::shared_ptr<client> _second_player;
    std::shared_ptr<reversi_game> _game;
    std::string _name;

public:
    lobby(std::string  name, const std::shared_ptr<client>& creator);
};
