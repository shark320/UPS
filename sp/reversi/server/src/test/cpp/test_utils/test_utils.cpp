#include <logger.h>
#include "test_utils.hpp"
#include <fmt/core.h>

static auto LOGGER = log4cxx::Logger::getLogger("unit_test");
namespace test_suit {
    void test_case(const std::function<void()> &_test_function, const std::string &tag) {
//    LOGGER->debug(fmt::format("Starting test case [{}]", tag));
        bool failed = false;
        try {
            _test_function();
        } catch (const std::exception &e) {
            failed = true;
            std::cerr << "\033[31mFailed:: " << e.what() << "\033[0m" << std::endl;
        }
        if (!failed) {
            LOGGER->debug(fmt::format("Successful test case [{}]", tag));
        } else {
            LOGGER->debug(fmt::format("Failed test case [{}]", tag));
        }

    }

    void test_bundle(
            const std::string &tag,
            const std::vector<std::function<void()>> &test_cases
    ) {
        LOGGER->debug(fmt::format("Starting test bundle [{}]", tag));
        for (const auto &test_case: test_cases) {
            test_case();
        }
        LOGGER->debug(fmt::format("Finished test bundle [{}]", tag));
    }


    void assert_true(bool condition, const std::string &message) {
        if (!condition) {
            throw std::runtime_error("Assertion failed: " + message);
        }
    }

    void assert_throw(const std::function<void()> &function, const std::string &message) {
        bool is_thrown = false;
        try {
            function();
        } catch (const std::exception &e) {
            is_thrown = true;
            LOGGER->debug(fmt::format("Exception thrown:  {}", e.what()));
        }
        if (!is_thrown) {
            throw std::runtime_error("Assertion failed: " + message);
        }
    }

    void assert_false(bool condition, const std::string &message) {
        test_suit::assert_true(!condition, message);
    }

    template<typename T>
    void assert_equals(const T &expected, const T &actual, const std::string &message) {
        if (expected != actual) {
            throw std::runtime_error(
                    fmt::format("Assertion failed: expected: {}, but got: {}. {}", expected, actual, message));
        }
    }

    void assert_equals_base(unsigned long const& expected, unsigned long const& actual, const std::string &message) {
        if (expected != actual) {
            throw std::runtime_error(
                    fmt::format("Assertion failed: expected: {}, but got: {}. {}", expected, actual, message));
        }
    }

    void assert_equals_base(const std::string &expected, const std::string &actual,
                                         const std::string &message) {
        if (expected != actual) {
            throw std::runtime_error(
                    fmt::format("Assertion failed: expected: {}, but got: {}. {}", expected, actual, message));
        }
    }
}