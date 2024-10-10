#pragma once

#include <string>
#include <memory>
#include "SimpleIni.h"


class connection_config {
private:
    std::string identifier = "";

public:
    explicit connection_config(const std::shared_ptr<CSimpleIniA>& ini_config);

    connection_config();

    void init(const std::shared_ptr<CSimpleIniA>& ini_config);

    [[nodiscard]] std::string get_identifier() const;

    [[nodiscard]] std::string to_string() const;
};

