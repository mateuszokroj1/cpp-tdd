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
    static constexpr uint32_t MAX_FRAMES_COUNT = 10;
    static constexpr uint32_t MAX_ROLLS_IN_GAME = 21;
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
        
        for(int frame_index = 0, roll_index = 0; frame_index < MAX_FRAMES_COUNT; ++frame_index)
        {
            const auto [score_in_frame, frame_size] = frame_score(roll_index);
            result += score_in_frame;

            if (is_strike(roll_index))
            {
                result += strike_bonus(roll_index);                 
            }
                        
            if (is_spare(roll_index))
            {
                result += spare_bonus(roll_index); 
            }
            
            roll_index += frame_size;
        }
        
        return result;
    }
private:
    bool is_spare(uint32_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index+1] == MAX_PINS_IN_FRAME;
    }

    bool is_strike(uint32_t roll_index) const
    {
        return pins_[roll_index] == MAX_PINS_IN_FRAME;
    }

    uint32_t spare_bonus(uint32_t roll_index) const
    {
        return pins_[roll_index+2];
    }

    uint32_t strike_bonus(uint32_t roll_index) const
    {
        return pins_[roll_index+1] + pins_[roll_index+2];
    }

    std::pair<uint32_t, uint8_t> frame_score(uint32_t roll_index) const
    {
        if (is_strike(roll_index))
            return std::pair{MAX_PINS_IN_FRAME, 1};
                   
        return std::pair{pins_[roll_index] + pins_[roll_index+1], 2};
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

    void roll_spare()
    {
        game.roll(5);
        game.roll(5);
    }

    void roll_strike()
    {
        game.roll(10);
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
    roll_spare();

    roll_many(1, 18);

    ASSERT_EQ(game.score(), 29);
}

TEST_F(BowlingGameTests, WhenStrike_TwoNextRollsAreDoubled)
{
    roll_strike();

    roll_many(1, 18);

    ASSERT_EQ(game.score(), 30);
}

TEST_F(BowlingGameTests, WhenSpareInLastFrame_ExtraRollAllowed)
{
    roll_many(1, 18);
    roll_spare();
    game.roll(2);

    ASSERT_EQ(game.score(), 30);
}


TEST_F(BowlingGameTests, WhenStrikeInLastFrame_TwoExtraRollsAllowed)
{
    roll_many(1, 18);
    roll_strike();
    game.roll(2);
    game.roll(1);

    ASSERT_EQ(game.score(), 31);
}

struct BowlingGameParams
{
    const char* test_description;
    std::vector<uint32_t> rolls;
    size_t expected_score;
};

std::ostream& operator<<(std::ostream& out, const BowlingGameParams& params)
{
    out << params.test_description;
    return out;
}

struct BowlingGameParamTests : ::testing::TestWithParam<BowlingGameParams>
{
    BowlingGame game; // SUT
};

TEST_P(BowlingGameParamTests, RealGameExamples)
{
    const BowlingGameParams param = GetParam();

    for(size_t pins : param.rolls)
    {
        game.roll(pins);
    }

    ASSERT_EQ(game.score(), param.expected_score);
}

BowlingGameParams params[] = {
    { "simple game - all rolls one pin", { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, 20 },
    { "simple game - different rolls", {0, 8, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 27},
    { "strike & spare", {10, 4, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 47},
    { "all spares & strike", {1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 1, 9, 10}, 119},
    { "perfect game", {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}, 300}
};

INSTANTIATE_TEST_SUITE_P(PackOfBowlingTests, BowlingGameParamTests, ::testing::ValuesIn(params));