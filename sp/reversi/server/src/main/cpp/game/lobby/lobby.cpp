
#include "lobby.hpp"

#include <utility>

lobby::lobby(std::string  name, const std::shared_ptr<client>& creator): _name(std::move(name)), _first_player(creator) {

}
