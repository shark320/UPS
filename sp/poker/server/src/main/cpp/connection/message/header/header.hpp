#pragma once

#include <string>
#include <memory>

#include "../enums/type.hpp"
#include "../enums/status.hpp"

class header {
private:
    std::string identifier;

    type _type = type::NULL_TYPE;

    subtype _subtype = subtype::NULL_SUBTYPE;

    status _status = status::NULL_STATUS;

    size_t length;

public:

    header(const std::string &identifier, type type, subtype subtype, status status, size_t length);

    [[nodiscard]] const std::string &get_identifier() const;

    [[nodiscard]] const type &get_type() const;

    [[nodiscard]] const subtype &get_subtype() const;

    [[nodiscard]] const status &get_status() const;

    [[nodiscard]] size_t get_length() const;

    void set_identifier(const std::string &identifier);

    void set_type(const type &type);

    void set_subtype(const subtype &subtype);

    void set_status(const status &status);

    void set_length(size_t length);

    [[nodiscard]] bool check_values() const;

    std::string construct() const;

    static std::shared_ptr<header> extract(const std::string& message);

    std::string to_string() const;
};