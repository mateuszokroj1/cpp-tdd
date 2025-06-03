#include <algorithm>
#include <string>
#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bowling.hpp"

using namespace std;

class BowlingGame
{
public:
    void roll(uint32_t)
    {        
    }

    uint32_t score() const
    {
        return 0;
    }
};

TEST(BowlingGameTests, WhenGameStarts_ScoreReturnsZero)
{
    BowlingGame game;

    ASSERT_EQ(game.score(), 0);
}

TEST(BowlingGameTests, WhenAllRollsInGutter_ScoreReturnsZero)
{
    BowlingGame game;
    
    const int MAX_ROLLS = 20;
    for(int i = 0; i < MAX_ROLLS; ++i)
    {
        game.roll(0);
    }

    ASSERT_EQ(game.score(), 0);
}