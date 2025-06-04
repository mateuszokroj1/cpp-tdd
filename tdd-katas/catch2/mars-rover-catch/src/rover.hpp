#ifndef ROVER_HPP
#define ROVER_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <limits>
#include <memory>
#include <ranges>
#include <string>
#include <vector>
#include <format>
#include <optional>

namespace TDD
{
    enum class Direction
    {
        North,
        East,
        South,
        West
    };

    std::ostream& operator<<(std::ostream& os, Direction direction)
    {
        switch (direction)
        {
        case Direction::North:
            os << "North";
            break;
        case Direction::South:
            os << "South";
            break;
        case Direction::East:
            os << "East";
            break;
        case Direction::West:
            os << "West";
            break;
        }
        return os;
    }

    struct Position
    {
        int x, y;
        Direction direction;

        bool operator==(const Position& other) const = default;
    };

    std::ostream& operator<<(std::ostream& os, const Position& pos)
    {
        return os << "Position(" << pos.x << ", " << pos.y << ", " << pos.direction << ")";
    }

    class UnknownCommandException : public std::runtime_error
    {
    public:
        explicit UnknownCommandException(const std::string& message)
            : std::runtime_error(message)
        {
        }
    };

    class Map
    {
        size_t max_width_;
        size_t max_height_;

    public:
        Map(size_t w, size_t h)
            : max_width_{w}
            , max_height_{h}
        {
        }

        std::pair<size_t, size_t> wrap(int x, int y)
        {
            size_t wrapped_x = (x + max_width_) % max_width_;
            size_t wrapped_y = (y + max_height_) % max_height_;
            auto f = std::format("({}, {})", wrapped_x, wrapped_y);
            return {wrapped_x, wrapped_y};
        }
    };

    class Rover
    {
        Position position_;
        std::optional<Map> map_;

    public:
        Rover(int x, int y, Direction direction, std::optional<Map> map = std::nullopt)
            : position_{x, y, direction}
            , map_{map}
        {
        }

        Position get_position() const
        {
            return position_;
        }

        void move_forward()
        {
            switch (position_.direction)
            {
            case Direction::North:
                position_.y += 1;
                break;
            case Direction::South:
                position_.y -= 1;
                break;
            case Direction::East:
                position_.x += 1;
                break;
            case Direction::West:
                position_.x -= 1;
                break;
            }

            wrap_coordinates();
        }

        void move_backward()
        {
            switch (position_.direction)
            {
            case Direction::North:
                position_.y -= 1;
                break;
            case Direction::South:
                position_.y += 1;
                break;
            case Direction::East:
                position_.x -= 1;
                break;
            case Direction::West:
                position_.x += 1;
                break;
            }

            wrap_coordinates();
        }

        void turn_left()
        {
            position_.direction = turn(position_.direction, TurnDirection::Left);
        }

        void turn_right()
        {
            position_.direction = turn(position_.direction, TurnDirection::Right);
        }

        void execute(const std::string& commands)
        {
            for (char cmd : commands)
            {
                switch (std::toupper(cmd))
                {
                case 'L':
                    turn_left();
                    break;
                case 'R':
                    turn_right();
                    break;
                case 'F':
                    move_forward();
                    break;
                case 'B':
                    move_backward();
                    break;
                default:
                    throw UnknownCommandException(std::string("Unknown command: ") + cmd);
                }
            }
        }

    private:
        void wrap_coordinates()
        {
            if (map_)
            {
                auto [wrapped_x, wrapped_y] = map_->wrap(position_.x, position_.y);
                position_.x = wrapped_x;
                position_.y = wrapped_y;
            }
        }

        inline constexpr static std::array directions_{
            Direction::North, Direction::East, Direction::South, Direction::West
        };

        enum class TurnDirection : int8_t
        {
            Left = -1,
            Right = 1
        };

        static Direction turn(Direction current_direction, TurnDirection turn_direction)
        {
            auto index = std::to_underlying(current_direction) + std::to_underlying(turn_direction);
            return directions_[index % std::size(directions_)];
        }
    };
}

#endif