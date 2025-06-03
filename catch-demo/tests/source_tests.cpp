#include "source.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

using namespace std;

struct Value
{
    int v;

    bool operator==(const Value&) const = default;

    friend Value operator+(const Value& a, const Value& b)
    {
        return Value{a.v + b.v};
    }

    friend std::ostream& operator<<(std::ostream& out, const Value& value)
    {
        out << "Value{" << value.v << "}";
        return out;
    }
};

TEST_CASE("Value: operator +", "[arithmetic][operators]")
{
    Value v1{42};
    Value v2{43};

    REQUIRE(v1 + v2 == Value{85});
}

TEST_CASE("pushing items to vector", "[vector][push]")
{
    vector<int> vec{3};

    CHECK(vec.size() == 1);
    CHECK(vec.capacity() == 1);

    SECTION("push_back")
    {
        vec.push_back(1);

        SECTION("size is increased")
        {
            REQUIRE(vec.size() == 2);
        }

        SECTION("capacity can be increased")
        {
            REQUIRE(vec.capacity() >= 2);
        }
    }
}

SCENARIO("pushing items to vector")
{
    GIVEN("vector with data")
    {
        vector<int> vec = {1, 2, 3};
        auto prev_size = vec.size();

        WHEN("push_back() an item")
        {
            vec.push_back(4);

            THEN("size is increased")
            {
                REQUIRE(vec.size() - prev_size == 1);
            }

            AND_THEN("capacity can be increased")
            {
                REQUIRE(vec.capacity() >= 4);
            }
        }
    }
}

struct BowlingGame
{
    size_t score_{};

    void roll(size_t pins)
    {
        score_ += pins;
    }

    size_t score() const
    {
        return score_;
    }
};

TEST_CASE("Simple game")
{
    BowlingGame game;

    const auto [pins, expected_score] = GENERATE(
        table<size_t, size_t>(
            {
                {1, 2},
                {2, 4},
                {3, 6}
            }
        )
    );

    DYNAMIC_SECTION("roll(" << pins << ")")
    {
        game.roll(pins);
        game.roll(pins);

        REQUIRE(game.score() == expected_score);
    }
}

TEST_CASE("Generators")
{
    auto i = GENERATE(1, 2);

    SECTION("one")
    {
        auto j = GENERATE(-3, -2);
        DYNAMIC_SECTION("test: " << i << " < " << j)
        {
            REQUIRE(j < i);
        }
    }

    SECTION("two")
    {
        auto k = GENERATE(4, 5, 6);
        REQUIRE(i != k);
    }
}

TEST_CASE("Generating random ints", "[example][generator]")
{
    SECTION("Deducing functions")
    {
        auto i = GENERATE(take(100, filter([](int i) { return i % 2 == 1; }, random(-100, 100))));
        REQUIRE(i > -100);
        REQUIRE(i < 100);
        REQUIRE(i % 2 == 1);
    }
}

unsigned int Factorial(unsigned int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorial") 
{
    CHECK(Factorial(0) == 0);
    // some more asserts..

    CHECK(Factorial(5) == 120);
    // some more asserts..

    // now let's benchmark:
    BENCHMARK("Factorial 0") {
        return Factorial(0);
    }; 

    BENCHMARK("Factorial 1") {
        return Factorial(1);
    };

    BENCHMARK("Factorial 3") {
        return Factorial(3);
    };

    BENCHMARK("Factorial 5") {
        return Factorial(5);
    };
}