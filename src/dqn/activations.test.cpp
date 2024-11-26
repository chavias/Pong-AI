#include "activations.hpp"
#include <gtest/gtest.h>
#include <cmath>
#include <limits>

// Test Suite for activationF
TEST(ActivationFunctionTest, ActivationF_HandlesPositiveInput)
{
    // Arrange
    double input = 1.0;
    double expected_output = 1 / (1 + std::exp(-std::max(0.0, input)));

    // Act
    double output = activationF(input);

    // Assert
    EXPECT_NEAR(output, expected_output, std::numeric_limits<double>::epsilon());
}

TEST(ActivationFunctionTest, ActivationF_HandlesNegativeInput)
{
    double input = -1.0;
    double expected_output = 1 / (1 + std::exp(-std::max(0.0, input)));

    double output = activationF(input);

    EXPECT_NEAR(output, expected_output, std::numeric_limits<double>::epsilon());
}

TEST(ActivationFunctionTest, ActivationF_HandlesZeroInput)
{
    double input = 0.0;
    double expected_output = 1 / (1 + std::exp(-std::max(0.0, input)));

    double output = activationF(input);

    EXPECT_NEAR(output, expected_output, std::numeric_limits<double>::epsilon());
}

// Test Suite for derActivationF
TEST(ActivationFunctionTest, DerActivationF_HandlesPositiveInput)
{
    double input = 0.8; // Simulate ReLU + Sigmoid input > 0
    double expected_output = 1 * (1 - 1);

    double output = derActivationF(input);

    EXPECT_EQ(output, expected_output);
}

TEST(ActivationFunctionTest, DerActivationF_HandlesNegativeInput)
{
    double input = -1.0; // Simulate ReLU + Sigmoid input <= 0
    double expected_output = 0;

    double output = derActivationF(input);

    EXPECT_EQ(output, expected_output);
}

TEST(ActivationFunctionTest, DerActivationF_HandlesZeroInput)
{
    double input = 0.0; // ReLU's zero case
    double expected_output = 0;

    double output = derActivationF(input);

    EXPECT_EQ(output, expected_output);
}

