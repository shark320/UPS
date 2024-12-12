#include "server_config.hpp"
#include "fmt/format.h"

static const std::string SERVER_SECTION = "server";
static const std::string PORT_KEY = "port";
static const std::string CLIENT_QUEUE_SIZE_KEY = "client_queue_size";
static const std::string HANDSHAKE_TIMEOUT_KEY = "handshake_timeout";
static const std::string TIMEOUT_CHECK_INTERVAL_KEY = "timeout_check_interval";
static const std::string LOGIN_TIMEOUT_KEY = "login_timeout";
static const std::string PING_TIMEOUT_KEY = "ping_timeout";

bool server_config::is_complete() const {
    return
    this->port != -1 &&
    this->client_queue_size != -1 &&
    this->handshake_timeout != -1 &&
    this->timeout_check_interval != -1 &&
    this->login_timeout != -1 &&
    this->ping_timeout != -1;
}

int server_config::get_port() const {
    return this->port;
}

int server_config::get_client_queue_size() const {
    return this->client_queue_size;
}

void server_config::init(const std::shared_ptr<CSimpleIniA> &ini_config) {
    this->port = (int) ini_config->GetLongValue(SERVER_SECTION.c_str(), PORT_KEY.c_str(), -1);
    if (this->port < 0 || this->port > 65535){
        throw std::invalid_argument("Invalid port configuration");
    }
    this->client_queue_size = (int) ini_config->GetLongValue(SERVER_SECTION.c_str(), CLIENT_QUEUE_SIZE_KEY.c_str(), -1);
    if (this->client_queue_size <= 0) {
        throw std::invalid_argument("Invalid client queue size configuration");
    }
    this->handshake_timeout = (int) ini_config->GetLongValue(SERVER_SECTION.c_str(), HANDSHAKE_TIMEOUT_KEY.c_str(), -1);
    if (this->handshake_timeout<0){
        throw std::invalid_argument("Invalid handshake timeout configuration");
    }
    this->timeout_check_interval = (int) ini_config->GetLongValue(SERVER_SECTION.c_str(), TIMEOUT_CHECK_INTERVAL_KEY.c_str(), -1);
    if (this->timeout_check_interval<0){
        throw std::invalid_argument("Invalid timeout check interval configuration");
    }
    this->login_timeout = (int) ini_config->GetLongValue(SERVER_SECTION.c_str(), LOGIN_TIMEOUT_KEY.c_str(), -1);
    if (this->login_timeout<0){
        throw std::invalid_argument("Invalid login timeout configuration");
    }
    this->ping_timeout = (int) ini_config->GetLongValue(SERVER_SECTION.c_str(), PING_TIMEOUT_KEY.c_str(), -1);
    if (this->ping_timeout<0){
        throw std::invalid_argument("Invalid ping timeout configuration");
    }
}

server_config::server_config() {}

server_config::server_config(const std::shared_ptr<CSimpleIniA> &ini_config) {
    init(ini_config);
}

std::string server_config::to_string() const {
    return fmt::format(
            "[port={}; client_queue_size={}; handshake_timeout={}, timeout_check_interval={}, login_timeout={}, ping_timeout={}]",
            this->port,
            this->client_queue_size,
            this->handshake_timeout,
            this->timeout_check_interval,
            this->login_timeout,
            this->ping_timeout
    );
}

int server_config::get_handshake_timeout() const {
    return this->handshake_timeout;
}

int server_config::get_timeout_check_interval() const {
    return this->timeout_check_interval;
}

int server_config::get_login_timeout() const {
    return this->login_timeout;
}

int server_config::get_ping_timeout() const {
    return this->ping_timeout;
}
