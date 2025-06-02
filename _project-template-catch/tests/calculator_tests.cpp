#include <algorithm>
#include <string>
#include <memory>
#include <catch2/catch_test_macros.hpp>

#include <calculator.hpp>

TEST_CASE("Addition of two numbers", "[add]")
{
    REQUIRE(add(1, 2) == 3);
    REQUIRE(add(0, 0) == 0);
    REQUIRE(add(-1, 1) == 0);
    REQUIRE(add(-1, -1) == -2);
}

