#pragma once

#include <string>
#include <vector>
#include <memory>

class object{
public:
    virtual ~object();

    [[nodiscard]] virtual std::string to_string() const;
};

class string: public object, public std::string{
public:
    using std::string::string;

    [[nodiscard]] std::string to_string() const override;
};

class integer: public object{
private:
    int _value;

public:
    integer();

    integer(int _value);

    integer(const integer &other);

    integer operator+(const integer& other) const ;

    integer operator-(const integer& other) const ;

    integer operator*(const integer& other) const ;

    integer operator/(const integer& other) const ;

    bool operator==(const integer& other) const;

    bool operator!=(const integer& other) const;

    integer& operator=(const integer& other);

    integer& operator=(const int& value);

    [[nodiscard]] std::string to_string() const override;

    int value();
};


class vector: public object, public std::vector<std::shared_ptr<object>>{
public:
    using std::vector<std::shared_ptr<object>>::vector;

    [[nodiscard]] std::string to_string() const override;
};

class boolean: public object {

private:
    bool _value;

public:
    boolean();

    boolean(bool _value);

    boolean(const boolean &other);

    bool operator==(const boolean& other) const;

    bool operator!=(const boolean& other) const;

    boolean& operator=(const boolean& other);

    boolean& operator=(const bool& value);

    [[nodiscard]] std::string to_string() const override;

    bool value();
};



