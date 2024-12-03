#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include "../../src/dqn/activations.hpp"
#include <gtest/gtest.h>
#include <Eigen/Dense>

// // Include the functions to be tested
// template <typename T>
// T derActivationF(const T& x) {
//     return (x.array() > 0).cast<typename T::Scalar>();
// }

// template <typename T>
// T activationF(const T& input) {
//     return input.array().max(0); // ReLU example
// }

// Test for activationF (ReLU)
TEST(ActivationFunctionTest, ReLUActivation) {
    // Test with scalar
    Eigen::Vector3f input(1.0f, -2.0f, 3.0f);
    Eigen::Vector3f expected_output(1.0f, 0.0f, 3.0f);
    EXPECT_EQ(activationF(input), expected_output);

    // Test with zero
    Eigen::Vector3f input_zero(0.0f, -1.0f, 0.0f);
    Eigen::Vector3f expected_output_zero(0.0f, 0.0f, 0.0f);
    EXPECT_EQ(activationF(input_zero), expected_output_zero);
}

// Test for derActivationF (ReLU Derivative)
TEST(DerivativeFunctionTest, ReLUDerivative) {
    // Test with positive, negative, and zero values
    Eigen::Vector3f input(1.0f, -2.0f, 0.0f);
    Eigen::Vector3f expected_output(1.0f, 0.0f, 0.0f);
    EXPECT_EQ(derActivationF(input), expected_output);

    // Test with all positive values
    Eigen::Vector3f input_positive(2.0f, 4.0f, 6.0f);
    Eigen::Vector3f expected_output_positive(1.0f, 1.0f, 1.0f);
    EXPECT_EQ(derActivationF(input_positive), expected_output_positive);

    // Test with all negative values
    Eigen::Vector3f input_negative(-1.0f, -3.0f, -5.0f);
    Eigen::Vector3f expected_output_negative(0.0f, 0.0f, 0.0f);
    EXPECT_EQ(derActivationF(input_negative), expected_output_negative);
}
