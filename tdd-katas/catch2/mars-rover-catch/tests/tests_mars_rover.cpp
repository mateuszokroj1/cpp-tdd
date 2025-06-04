#include "rover.hpp"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/trompeloeil.hpp>
#include <format>
#include <optional>
#include <ranges>
#include <string>

using namespace std;
using namespace TDD;

// Custom string maker for std::pair to work with Catch2
namespace Catch
{
    template <typename T1, typename T2>
    struct StringMaker<std::pair<T1, T2>>
    {
        static std::string convert(const std::pair<T1, T2>& p)
        {
            return "(" + Catch::Detail::stringify(p.first) + ", " + Catch::Detail::stringify(p.second) + ")";
        }
    };
}

TEST_CASE("Map - wraps coordinates")
{
    Map map{10, 20};

    auto [start, expected] = GENERATE(
        std::pair{std::pair{10, 20}, std::pair{0, 0}},
        std::pair{std::pair{15, 25}, std::pair{5, 5}},
        std::pair{std::pair{-1, -1}, std::pair{9, 19}});

    auto [x, y] = start;

    auto result = map.wrap(x, y);
    REQUIRE(result == expected);
}

CATCH_REGISTER_ENUM(Direction, Direction::North, Direction::South, Direction::East, Direction::West);

TEST_CASE("Rover initialized with coord and direction")
{
    Rover rover(5, 5, Direction::North);

    SECTION("reports current postion")
    {
        REQUIRE(rover.get_position() == Position{5, 5, Direction::North});
    }
}

TEST_CASE("Rover - move forward")
{
    auto [start_direction, expected_pos] = GENERATE(
        std::pair{Direction::North, Position{5, 6, Direction::North}},
        std::pair{Direction::South, Position{5, 4, Direction::South}},
        std::pair{Direction::East, Position{6, 5, Direction::East}},
        std::pair{Direction::West, Position{4, 5, Direction::West}});

    DYNAMIC_SECTION("Rover moves forward from " << start_direction)
    {
        Rover rover(5, 5, start_direction);

        rover.move_forward();

        DYNAMIC_SECTION("Ends on position " << expected_pos)
        {
            REQUIRE(rover.get_position() == expected_pos);
        }
    }
}

TEST_CASE("Rover - move backward")
{
    auto [start_direction, expected_pos] = GENERATE(
        std::pair{Direction::North, Position{5, 4, Direction::North}},
        std::pair{Direction::South, Position{5, 6, Direction::South}},
        std::pair{Direction::East, Position{4, 5, Direction::East}},
        std::pair{Direction::West, Position{6, 5, Direction::West}});

    DYNAMIC_SECTION("Rover moves forward from " << start_direction)
    {
        Rover rover(5, 5, start_direction);

        rover.move_backward();

        DYNAMIC_SECTION("Ends on position " << expected_pos)
        {
            REQUIRE(rover.get_position() == expected_pos);
        }
    }
}

TEST_CASE("Rover - turn left")
{
    auto [start_direction, expected_direction] = GENERATE(
        std::pair{Direction::North, Direction::West},
        std::pair{Direction::South, Direction::East},
        std::pair{Direction::East, Direction::North},
        std::pair{Direction::West, Direction::South});

    DYNAMIC_SECTION("Rover turns left from " << start_direction)
    {
        Rover rover(5, 5, start_direction);

        rover.turn_left();

        DYNAMIC_SECTION("Ends facing " << expected_direction)
        {
            REQUIRE(rover.get_position().direction == expected_direction);
        }
    }
}

TEST_CASE("Rover - turn right")
{
    auto [start_direction, expected_direction] = GENERATE(
        std::pair{Direction::North, Direction::East},
        std::pair{Direction::South, Direction::West},
        std::pair{Direction::East, Direction::South},
        std::pair{Direction::West, Direction::North});

    DYNAMIC_SECTION("Rover turns right from " << start_direction)
    {
        Rover rover(5, 5, start_direction);

        rover.turn_right();

        DYNAMIC_SECTION("Ends facing " << expected_direction)
        {
            REQUIRE(rover.get_position().direction == expected_direction);
        }
    }
}

TEST_CASE("Rover - executes list of commands")
{
    Rover rover(5, 5, Direction::East);

    SECTION("Uppercase set of commands")
    {
        rover.execute("FFLBR");

        REQUIRE(rover.get_position() == Position(7, 4, Direction::East));
    }

    SECTION("set of commands - case insensitive")
    {
        rover.execute("FfLBr");

        REQUIRE(rover.get_position() == Position(7, 4, Direction::East));
    }

    SECTION("Unknown command")
    {
        SECTION("throws exception")
        {
            REQUIRE_THROWS_AS(rover.execute("FFxFFF"), UnknownCommandException);
        }

        SECTION("rover stops and reports position")
        {
            try
            {
                rover.execute("FFxFFF");
            }
            catch (...)
            {
            }

            REQUIRE(rover.get_position() == Position(7, 5, Direction::East));
        }
    }
}

TEST_CASE("Rover - wrapping coordinates")
{
    Map mars_map{10, 10};

    SECTION("Rover wraps coordinates when moving north")
    {
        Rover rover(5, 9, Direction::North, mars_map);

        rover.move_forward();

        REQUIRE(rover.get_position() == Position(5, 0, Direction::North));
    }

    SECTION("Rover wraps coordinates when moving south")
    {
        Rover rover(5, 0, Direction::South, mars_map);

        rover.move_forward();

        REQUIRE(rover.get_position() == Position(5, 9, Direction::South));
    }

    SECTION("Rover wraps coordinates when moving east")
    {
        Rover rover(9, 5, Direction::East, mars_map);

        rover.move_forward();

        REQUIRE(rover.get_position() == Position(0, 5, Direction::East));
    }

    SECTION("Rover wraps coordinates when moving west")
    {
        Rover rover(5, 9, Direction::North, mars_map);

        rover.move_forward();

        REQUIRE(rover.get_position() == Position(5, 0, Direction::North));
    }
}
