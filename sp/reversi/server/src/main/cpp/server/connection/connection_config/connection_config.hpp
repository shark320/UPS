#pragma once

#include <string>
#include <memory>
#include "SimpleIni.h"


class connection_config {
private:
    std::string identifier = "";

    bool handshake_required = true;

public:
    explicit connection_config(const std::shared_ptr<CSimpleIniA>& ini_config);

    connection_config();

    void init(const std::shared_ptr<CSimpleIniA>& ini_config);

    [[nodiscard]] std::string get_identifier() const;

    [[nodiscard]] bool is_handshake_required() const;

    [[nodiscard]] std::string to_string() const;
};

