#include "player.hpp"

player::player(const std::shared_ptr<client>& client, player_code player_code):_player_code(player_code), _client(client) {

}

std::shared_ptr<client> player::get_client() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    return this->_client;
}

player_code player::get_player_code() const {
    std::shared_lock<std::shared_mutex> shared_lock(*this->shared_mutex);
    return this->_player_code;
}
