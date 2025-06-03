#include <algorithm>
#include <string>
#include <memory>
#include <array>
#include <numeric>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bowling.hpp"

using namespace std;

class BowlingGame
{    
    static constexpr uint32_t MAX_PINS_IN_FRAME = 10;
    static constexpr uint32_t MAX_ROLLS_IN_GAME = 20;
    std::array<uint32_t, MAX_ROLLS_IN_GAME> pins_{};
    uint32_t roll_count_{};
public:
    void roll(uint32_t pins)
    {        
        pins_[roll_count_] = pins;
        ++roll_count_;
    }

    uint32_t score() const
    {        
        uint32_t result = 0;

        const uint32_t frame_size = 2;
        for(int roll_index = 0; roll_index < MAX_ROLLS_IN_GAME; roll_index += frame_size)
        {
            if (is_spare(roll_index))
            {
                result += spare_bonus(roll_index); 
            }

            result += frame_score(roll_index);
        }
        
        return result;
    }
private:
    bool is_spare(uint32_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index+1] == MAX_PINS_IN_FRAME;
    }

    uint32_t spare_bonus(uint32_t roll_index) const
    {
        return pins_[roll_index+2];
    }

    uint32_t frame_score(uint32_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index+1];
    }
};

class BowlingGameTests : public ::testing::Test
{
protected:
    inline static const uint32_t MAX_ROLLS = 20;

    BowlingGame game;

    // void SetUp() override
    // {}

    // void TearDown() override
    // {}

    void roll_many(uint32_t pins, uint32_t count = MAX_ROLLS)
    {
        for(int i = 0; i < count; ++i)
        {
            game.roll(pins);
        }
    }
};

TEST_F(BowlingGameTests, WhenGameStarts_ScoreReturnsZero)
{
    ASSERT_EQ(game.score(), 0);
}

TEST_F(BowlingGameTests, WhenAllRollsInGutter_ScoreReturnsZero)
{   
    roll_many(0, MAX_ROLLS);

    ASSERT_EQ(game.score(), 0);
}

TEST_F(BowlingGameTests, WhenAllRollsWithoutBonus_ScoreIsSumOfRolledPins)
{
    roll_many(1, MAX_ROLLS);

    ASSERT_EQ(game.score(), 20);
}

TEST_F(BowlingGameTests, WhenSpare_NextRollIsDoubled)
{
    game.roll(5);
    game.roll(5); // spare

    roll_many(1, 18);

    ASSERT_EQ(game.score(), 29);
}

