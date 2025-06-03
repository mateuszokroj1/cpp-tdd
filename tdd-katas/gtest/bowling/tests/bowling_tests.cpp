#include <algorithm>
#include <string>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bowling.hpp"

using namespace std;

class BowlingGame
{
    uint32_t score_ = 0;
public:
    void roll(uint32_t pins)
    {        
        score_ += pins;
    }

    uint32_t score() const
    {
        return score_;
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