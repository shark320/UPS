#pragma once
#include <functional>
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
namespace test_suit {
    void test_case(const std::function<void()> &_test_function, const std::string &tag);


    void test_bundle(
            const std::string &tag,
            const std::vector<std::function<void()>>& test_cases
    );

    void assert_true(bool condition, const std::string &message = "");

    void assert_false(bool condition, const std::string &message = "");

    void assert_throw(const std::function<void()>& function, const std::string &message = "");


    template<typename T>
    void assert_equals(const T &expected, const T &actual, const std::string &message = "");

    void assert_equals_base(unsigned long const& expected, unsigned long const& actual, const std::string &message = "");

    void assert_equals_base(const std::string &expected, const std::string &actual, const std::string &message = "");
}
