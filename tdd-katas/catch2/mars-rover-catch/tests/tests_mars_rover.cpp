#include "rover.hpp"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/trompeloeil.hpp>

using namespace std;
using namespace TDD;

constexpr Coordinates start_coordinates{ .x = 0, .y = 0, .direction = Direction::N };

SCENARIO("Rover - Init")
{
  GIVEN("Rover - initialized with start coordinates")
  {
	Rover rover(start_coordinates);

	WHEN("Position is requested")
	{
	  auto pos = rover.getPosition();

	  THEN("current position is returned")
	  {
		REQUIRE(pos == start_coordinates);
	  }
	}
  }
}

using StartEndPos = std::pair<Coordinates, Coordinates>;

SCENARIO("Rover - moving forward")
{
  auto [start_coords, expected_coords] = GENERATE(
	StartEndPos{ Coordinates{.x = 5, .y = 5, .direction = Direction::N}, Coordinates{.x = 5, .y = 6, .direction = Direction::N} },
	StartEndPos{ Coordinates{.x = 4, .y = -10, .direction = Direction::E}, Coordinates{.x = 5, .y = -10, .direction = Direction::E} },
	StartEndPos{ Coordinates{.x = -5, .y = 10, .direction = Direction::S}, Coordinates{.x = -5, .y = 9, .direction = Direction::S} },
	StartEndPos{ Coordinates{.x = -10, .y = 2, .direction = Direction::W}, Coordinates{.x = -11, .y = 2, .direction = Direction::W} });

  GIVEN("Rover - initialized with position " << start_coords)
  {
	Rover rover(start_coords);

	WHEN("Moved forward")
	{
	  rover.moveForward();

	  THEN("Current position changed to " << expected_coords)
	  {
		REQUIRE(rover.getPosition() == expected_coords);
	  }
	}
  }
}

SCENARIO("Rover - moving backward")
{
  auto [start_coords, expected_coords] = GENERATE(
	StartEndPos{ Coordinates{.x = 5, .y = 5, .direction = Direction::N}, Coordinates{.x = 5, .y = 4, .direction = Direction::N} },
	StartEndPos{ Coordinates{.x = 4, .y = -10, .direction = Direction::E}, Coordinates{.x = 3, .y = -10, .direction = Direction::E} },
	StartEndPos{ Coordinates{.x = -5, .y = 10, .direction = Direction::S}, Coordinates{.x = -5, .y = 11, .direction = Direction::S} },
	StartEndPos{ Coordinates{.x = -10, .y = 2, .direction = Direction::W}, Coordinates{.x = -9, .y = 2, .direction = Direction::W} });

  GIVEN("Rover - initialized with position " << start_coords)
  {
	Rover rover(start_coords);

	WHEN("Moved backward")
	{
	  rover.moveBackward();

	  THEN("Current position changed to " << expected_coords)
	  {
		REQUIRE(rover.getPosition() == expected_coords);
	  }
	}
  }
}

using DirectionPair = std::pair<Direction, Direction>;
SCENARIO("Rover - turn right")
{
  auto [start_dir, expected_dir] = GENERATE(
	DirectionPair{ Direction::N, Direction::E },
	DirectionPair{ Direction::E, Direction::S },
	DirectionPair{ Direction::S, Direction::W },
	DirectionPair{ Direction::W, Direction::N });

  Coordinates start_coords;
  start_coords.direction = start_dir;

  GIVEN("Rover - initialized with position " << start_coords)
  {
	Rover rover(start_coords);

	WHEN("Turn Right")
	{
	  rover.turnRight();

	  Coordinates expected_coords;
	  expected_coords.direction = expected_dir;

	  THEN("Current direction changed to " << expected_coords)
	  {
		REQUIRE(rover.getPosition() == expected_coords);
	  }
	}
  }
}

SCENARIO("Rover - turn left")
{
  auto [start_dir, expected_dir] = GENERATE(
	DirectionPair{ Direction::N, Direction::W },
	DirectionPair{ Direction::E, Direction::N },
	DirectionPair{ Direction::S, Direction::E },
	DirectionPair{ Direction::W, Direction::S });

  Coordinates start_coords;
  start_coords.direction = start_dir;

  GIVEN("Rover - initialized with position " << start_coords)
  {
	Rover rover(start_coords);

	WHEN("Turn Left")
	{
	  rover.turnLeft();

	  Coordinates expected_coords;
	  expected_coords.direction = expected_dir;

	  THEN("Current direction changed to " << expected_coords)
	  {
		REQUIRE(rover.getPosition() == expected_coords);
	  }
	}
  }
}

const Limited2dPlane planet_limit{ .x_min = -100, .y_min = -100, .x_max = 100, .y_max = 100 };

SCENARIO("Rover - when drives to planet coordinates limit - move forward")
{
  auto [start_coords, expected_coords] = GENERATE(
	StartEndPos{ Coordinates{.x = 1, .y = 100, .direction = Direction::N}, Coordinates{.x = 1, .y = -100, .direction = Direction::N} },
	StartEndPos{ Coordinates{.x = 100, .y = -100, .direction = Direction::E}, Coordinates{.x = -100, .y = -100, .direction = Direction::E} },
	StartEndPos{ Coordinates{.x = -100, .y = -100, .direction = Direction::S}, Coordinates{.x = -100, .y = 100, .direction = Direction::S} },
	StartEndPos{ Coordinates{.x = -100, .y = 2, .direction = Direction::W}, Coordinates{.x = 100, .y = 2, .direction = Direction::W} });

  GIVEN("Rover - initialized with position " << start_coords)
  {
	Rover rover(start_coords, planet_limit);

	WHEN("Moved forward")
	{
	  rover.moveForward();

	  THEN("Current position changed to " << expected_coords)
	  {
		REQUIRE(rover.getPosition() == expected_coords);
	  }
	}
  }
}

SCENARIO("Rover - when drives to planet coordinates limit - move backward")
{
  auto [start_coords, expected_coords] = GENERATE(
	StartEndPos{ Coordinates{.x = 1, .y = -100, .direction = Direction::N}, Coordinates{.x = 1, .y = 100, .direction = Direction::N} },
	StartEndPos{ Coordinates{.x = -100, .y = -100, .direction = Direction::E}, Coordinates{.x = 100, .y = -100, .direction = Direction::E} },
	StartEndPos{ Coordinates{.x = -100, .y = 100, .direction = Direction::S}, Coordinates{.x = -100, .y = -100, .direction = Direction::S} },
	StartEndPos{ Coordinates{.x = 100, .y = 2, .direction = Direction::W}, Coordinates{.x = -100, .y = 2, .direction = Direction::W} });

  GIVEN("Rover - initialized with position " << start_coords)
  {
	Rover rover(start_coords, planet_limit);

	WHEN("Moved backward")
	{
	  rover.moveBackward();

	  THEN("Current position changed to " << expected_coords)
	  {
		REQUIRE(rover.getPosition() == expected_coords);
	  }
	}
  }
}

using CommandAndCoords = std::pair<std::string, Coordinates>;

SCENARIO("Rover - command")
{
  auto [command_sequence, expected_coords] = GENERATE(CommandAndCoords{ "LLFFBBRR" , start_coordinates }, CommandAndCoords{ "FFFFFL" , {.x = 0, .y = 5, .direction = Direction::W } });

  GIVEN("Rover - initialized with start coordinates " << start_coordinates)
  {
	Rover rover(start_coordinates);

	WHEN("Command sequence is requested: " << command_sequence)
	{
	  rover.runCommandSequence(command_sequence);

	  THEN("Current coordinate is returned")
	  {
		auto result = rover.getPosition();

		REQUIRE(result == expected_coords);
	  }
	}
  }
}

SCENARIO("Rover - unsupported command")
{
  auto command = GENERATE(std::string("LLFxM12c 986."), std::string("LLX12 .';FRR"));

	GIVEN("Rover - initialized with start coordinates " << start_coordinates)
	{
	  Rover rover(start_coordinates);

		WHEN("Unsupported command sequence is requested: " << command)
		{
			THEN("Should throw exception")
			{
			  REQUIRE_THROWS_AS(rover.runCommandSequence(command), NotSupportedCommandException);
			}
		}
	}
}

