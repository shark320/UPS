//
// Created by vladi on 08.10.2024.
//

#include "client_info.hpp"

int client_info::get_socket() {
    return this->socket;
}

client_info::client_info(int socket) {
    this->socket = socket;
}
