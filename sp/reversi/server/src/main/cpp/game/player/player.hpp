#pragma once


#include "../../server/connection/client/client.hpp"

class player {
private:
    std::shared_ptr<client> _client;
    player_code _player_code;

    std::shared_ptr<std::shared_mutex> shared_mutex = std::make_shared<std::shared_mutex>();
public:
    explicit player(const std::shared_ptr<client>& client, player_code player_code);

    [[nodiscard]] std::shared_ptr<client> get_client() const;

    player_code get_player_code() const;
};
