#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include "activations.hpp"

// Define the test fixture class
class ActivationFunctionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

// Test `activationF` with ReLU and Sigmoid combined
TEST_F(ActivationFunctionTest, ActivationF_BasicCases) {
    // Test positive input
    EXPECT_DOUBLE_EQ(activationF(3.0), 0.9525741268224334); // Sigmoid(3)
    
    // Test negative input
    EXPECT_DOUBLE_EQ(activationF(-2.0), 0.5);              // ReLU clips to 0; Sigmoid(0) = 0.5
    
    // Test zero input
    EXPECT_DOUBLE_EQ(activationF(0.0), 0.5);               // Sigmoid(0)
}

// Test `derActivationF` (derivative of activation function)
TEST_F(ActivationFunctionTest, DerActivationF_BasicCases) {
    // Test positive input
    EXPECT_EQ(derActivationF(3.0), 0); // Sigmoid derivative is 0 * (1 - 0) due to ReLU
    
    // Test negative input
    EXPECT_EQ(derActivationF(-2.0), 0); // ReLU derivative = 0; no contribution to Sigmoid

    // Test zero input
    EXPECT_EQ(derActivationF(0.0), 0); // ReLU allows 0; Sigmoid derivative is 0 * (1 - 0)
}

// // Extended test for floating-point precision
// TEST_F(ActivationFunctionTest, ActivationF_Precision) {
//     // Test with small positive input
//     EXPECT_NEAR(activationF(0.001), 0.50025, 1e-5); // Sigmoid of small positive numbers approaches 0.5

//     // Test with small negative input
//     EXPECT_NEAR(activationF(-0.001), 0.49975, 1e-5); // Sigmoid of small negative numbers also approaches 0.5
// }

// // Extended test for boundary conditions
// TEST_F(ActivationFunctionTest, DerActivationF_Precision) {
//     // Test with very large input
//     double large = 1e6;
//     EXPECT_EQ(derActivationF(large), 0); // Sigmoid derivative saturates due to ReLU

//     // Test with very small negative input
//     double smallNegative = -1e6;
//     EXPECT_EQ(derActivationF(smallNegative), 0); // Clipped by ReLU
// }