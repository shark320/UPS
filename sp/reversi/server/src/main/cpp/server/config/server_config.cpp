#include "server_config.hpp"
#include "fmt/format.h"

static const std::string SERVER_SECTION = "server";
static const std::string PORT_KEY = "port";
static const std::string CLIENT_QUEUE_SIZE_KEY = "client_queue_size";
static const std::string HANDSHAKE_TIMEOUT_KEY = "handshake_timeout";

bool server_config::is_complete() const {
    return this->port != -1 && this->client_queue_size != -1 && this->handshake_timeout != -1;
}

int server_config::get_port() const {
    return this->port;
}

int server_config::get_client_queue_size() const {
    return this->client_queue_size;
}

void server_config::init(const std::shared_ptr<CSimpleIniA> &ini_config) {
    this->port = (int) ini_config->GetLongValue(SERVER_SECTION.c_str(), PORT_KEY.c_str(), -1);
    this->client_queue_size = (int) ini_config->GetLongValue(SERVER_SECTION.c_str(), CLIENT_QUEUE_SIZE_KEY.c_str(), -1);
    this->handshake_timeout = (int) ini_config->GetLongValue(SERVER_SECTION.c_str(), HANDSHAKE_TIMEOUT_KEY.c_str(), -1);
}

server_config::server_config() {}

server_config::server_config(const std::shared_ptr<CSimpleIniA> &ini_config) {
    init(ini_config);
}

std::string server_config::to_string() const {
    return fmt::format(
            "[port={}; client_queue_size={}; handshake_timeout={}]",
            this->port,
            this->client_queue_size,
            this->handshake_timeout
    );
}

int server_config::get_handshake_timeout() const {
    return this->handshake_timeout;
}
