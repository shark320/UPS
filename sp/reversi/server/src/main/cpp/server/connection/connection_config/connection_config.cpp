#include "connection_config.hpp"
#include "fmt/format.h"

static const std::string CONNECTION_SECTION = "Connection";

static const std::string IDENTIFIER_KEY = "identifier";

static const std::string HANDSHAKE_REQUIRED_KEY = "handshake_required";

void connection_config::init(const std::shared_ptr<CSimpleIniA> &ini_config) {
    this->identifier = ini_config->GetValue(CONNECTION_SECTION.c_str(), IDENTIFIER_KEY.c_str(), "");
    this->handshake_required = ini_config->GetBoolValue(CONNECTION_SECTION.c_str(), HANDSHAKE_REQUIRED_KEY.c_str(), true);
}

connection_config::connection_config(const std::shared_ptr<CSimpleIniA> &ini_config) {
    init(ini_config);
}

connection_config::connection_config() = default;

std::string connection_config::get_identifier() const {
    return this->identifier;
}

std::string connection_config::to_string() const {
    return fmt::format(
            "[identifier={}, handshake_required={}]",
            this->identifier,
            this->handshake_required ? "true" : "false"
    );
}

bool connection_config::is_handshake_required() const {
    return this->handshake_required;
}
