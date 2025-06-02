#include <algorithm>
#include <string>
#include <memory>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <calculator.hpp>

using namespace std;

TEST(CalculatorTests, AddReturnsSumOfTwoArguments)
{
    EXPECT_EQ(add(1, 2), 3);
}