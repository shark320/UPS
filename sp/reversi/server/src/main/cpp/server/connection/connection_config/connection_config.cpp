#include "connection_config.hpp"
#include "fmt/format.h"

static const std::string CONNECTION_SECTION = "Connection";

static const std::string IDENTIFIER_KEY = "identifier";

void connection_config::init(const std::shared_ptr<CSimpleIniA> &ini_config) {
    this->identifier = ini_config->GetValue(CONNECTION_SECTION.c_str(), IDENTIFIER_KEY.c_str(), "");
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
            "[identifier={}]",
            this->identifier
    );
}
