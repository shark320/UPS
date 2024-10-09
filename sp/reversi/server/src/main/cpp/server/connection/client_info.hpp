#pragma once

//TODO: rename
class client_info {
private:
    int socket = -1;

public:
    client_info(int socket);

    int get_socket();
};

