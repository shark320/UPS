#include <memory>
#include <fmt/core.h>
#include "header_test.hpp"
#include "../../../../test_utils/test_utils.hpp"
#include "../../../../../../main/cpp/server/connection/message/header/header.hpp"

using namespace test_suit;

//RVSI 10 GET PING
static auto header_str_empty_ok = "00000000";
static auto header_str_10_ok = "RVSI0010101200";

void header_test::header_constructor_test() {
    std::shared_ptr<header> header_empty = std::make_shared<header>();
    assert_equals_base("", header_empty->get_identifier());
    assert_equals_base((size_t)0, header_empty->get_length());
    assert_equals(status::NULL_STATUS, header_empty->get_status());
    std::shared_ptr<header> header_ok = std::make_shared<header>("RVSI", type::GET, subtype::PING, status::OK, 10);
    fmt::println("{}", header_ok->construct());
    assert_equals_base(header_str_10_ok, header_ok->construct());
}

void header_test::header_construct_test(){
    std::shared_ptr<header> header_empty = std::make_shared<header>();
    assert_throw([header_empty]() { header_empty->construct(); }, "");
    std::shared_ptr<header> header_ok = std::make_shared<header>("RVSI", type::GET, subtype::PING, status::OK, 10);
    fmt::println("{}", header_ok->construct());
    assert_equals_base(header_str_10_ok, header_ok->construct());
}

void header_test::header_test_bundle() {

    test_bundle(
            "Header Unit Tests",
             {
                [](){test_case(header_test::header_constructor_test, "Constructor Tests");}
             }
    );
}

