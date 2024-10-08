#pragma once

#include <memory>
#include "SimpleIni.h"

class server_config {
private:
    int port;
    int client_queue_size;
public:
    bool is_complete();

    int get_port();

    int get_client_queue_size();

    void init(std::shared_ptr<CSimpleIniA> ini_config);
};

