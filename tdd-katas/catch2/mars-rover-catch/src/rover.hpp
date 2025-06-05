#ifndef ROVER_HPP
#define ROVER_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <ranges>
#include <string>
#include <vector>
#include <memory>

namespace TDD
{
  class NotSupportedCommandException : std::invalid_argument
  {
  public:
	NotSupportedCommandException() : std::invalid_argument("Not supported command") {}
  };

  enum struct Direction : uint8_t
  {
	N = 0,
	E,
	S,
	W
  };

  inline std::ostream& operator<<(std::ostream& stream, const Direction& direction)
  {
	switch (direction)
	{
	case Direction::N:
	  stream << "N";
	  break;
	case Direction::E:
	  stream << "E";
	  break;
	case Direction::W:
	  stream << "W";
	  break;
	case Direction::S:
	  stream << "S";
	  break;
	}

	return stream;
  }

  struct Coordinates
  {
	double x, y;
	Direction direction;

	bool operator==(const Coordinates& other) const = default;
  };

  inline std::ostream& operator<<(std::ostream& stream, const Coordinates& coords)
  {
	stream << "{x = " << coords.x << ", y = " << coords.y << ", dir = " << coords.direction;

	return stream;
  }

  struct Limited2dPlane
  {
	double x_min = NAN;
	double y_min = NAN;
	double x_max = NAN;
	double y_max = NAN;

	[[nodiscard]] bool isUnlimited() const
	{
	  return std::isnan(x_min) && std::isnan(y_min) && std::isnan(x_max) && std::isnan(y_max);
	}

	bool operator==(const Limited2dPlane&) const = default;
  };

  inline std::ostream& operator<<(std::ostream& stream, const Limited2dPlane& plane)
  {
	stream << "{ min(" << plane.x_min << ", " << plane.y_min << "), max(" << plane.x_max << ", " << plane.y_max << ") }";

	return stream;
  }

  class Rover
  {
  public:
	Rover(const Coordinates& initial_coordinates, const Limited2dPlane& planet_limit = {}) : current_coordinates(initial_coordinates), planet_limit(planet_limit) {}

	Coordinates getPosition() const
	{
	  return current_coordinates;
	}

	void moveForward()
	{
	  switch (current_coordinates.direction)
	  {
	  case Direction::N:
		++current_coordinates.y;

		if (!planet_limit.isUnlimited())
		{
		  if (current_coordinates.y > planet_limit.y_max)
			current_coordinates.y = planet_limit.y_min;
		}

		break;
	  case Direction::E:
		++current_coordinates.x;

		if (!planet_limit.isUnlimited())
		{
		  if (current_coordinates.x > planet_limit.x_max)
			current_coordinates.x = planet_limit.x_min;
		}

		break;
	  case Direction::S:
		--current_coordinates.y;

		if (!planet_limit.isUnlimited())
		{
		  if (current_coordinates.y < planet_limit.y_min)
			current_coordinates.y = planet_limit.y_max;
		}

		break;
	  case Direction::W:
		--current_coordinates.x;

		if (!planet_limit.isUnlimited())
		{
		  if (current_coordinates.x < planet_limit.x_min)
			current_coordinates.x = planet_limit.x_max;
		}

		break;
	  }
	}

	void moveBackward()
	{
	  switch (current_coordinates.direction)
	  {
	  case Direction::N:
		--current_coordinates.y;

		if (!planet_limit.isUnlimited())
		{
		  if (current_coordinates.y < planet_limit.y_min)
			current_coordinates.y = planet_limit.y_max;
		}

		break;
	  case Direction::E:
		--current_coordinates.x;

		if (!planet_limit.isUnlimited())
		{
		  if (current_coordinates.x < planet_limit.x_min)
			current_coordinates.x = planet_limit.x_max;
		}

		break;
	  case Direction::S:
		++current_coordinates.y;

		if (!planet_limit.isUnlimited())
		{
		  if (current_coordinates.y > planet_limit.y_max)
			current_coordinates.y = planet_limit.y_min;
		}

		break;
	  case Direction::W:
		++current_coordinates.x;

		if (!planet_limit.isUnlimited())
		{
		  if (current_coordinates.x > planet_limit.x_max)
			current_coordinates.x = planet_limit.x_min;
		}

		break;
	  }
	}

	void turnRight()
	{
	  switch (current_coordinates.direction)
	  {
	  case Direction::N:
		current_coordinates.direction = Direction::E;
		break;
	  case Direction::E:
		current_coordinates.direction = Direction::S;
		break;
	  case Direction::S:
		current_coordinates.direction = Direction::W;
		break;
	  case Direction::W:
		current_coordinates.direction = Direction::N;
		break;
	  }
	}

	void turnLeft()
	{
	  switch (current_coordinates.direction)
	  {
	  case Direction::N:
		current_coordinates.direction = Direction::W;
		break;
	  case Direction::E:
		current_coordinates.direction = Direction::N;
		break;
	  case Direction::S:
		current_coordinates.direction = Direction::E;
		break;
	  case Direction::W:
		current_coordinates.direction = Direction::S;
		break;
	  }
	}

	void runCommandSequence(const std::string& sequence)
	{
	  for (char command : sequence)
	  {
		runCommand(command);
	  }
	}

	void runCommand(char command)
	{
	  switch (command) {
	  case 'F':
		moveForward();
		break;
	  case 'B':
		moveBackward();
		break;
	  case 'L':
		turnLeft();
		break;
	  case 'R':
		turnRight();
		break;
	  default:
		throw NotSupportedCommandException();
	  }
	}

  private:
	Coordinates current_coordinates;
	Limited2dPlane planet_limit;
  };
}

#endif