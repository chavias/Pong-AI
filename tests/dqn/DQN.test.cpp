#include <gtest/gtest.h>
#include "../../src/dqn/DQN.hpp"
#include <iostream>
#include <memory>
#include <Eigen/Dense>


// Test for the Memory class
class DQNTest : public ::testing::Test {
protected:
    // Define the memory capacity for the tests
    static constexpr size_t memory_capacity = 5;
    
    // Instance of Memory class zd
    std::unique_ptr<Agent> agent;
    
    void SetUp() override
    {
        agent = std::make_unique<Agent>(21, 7, 3, 21);  // Initialize the unique_ptr
    }

};


TEST_F(DQNTest, Normalization)
{
    Eigen::Matrix<float, 6, 1> variables;
    variables << 400.0f, 300.0f, 5.0f, 5.0f, 300.0f, 300.0f;

    Eigen::Matrix<float, 6, 1> expected_normalized;
    expected_normalized << 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f;

    // Compute normalized variables inside the function
    const float SCREEN_WIDTH = 800.0f, SCREEN_HEIGHT = 600.0f, BALL_SPEED = 10.0f;
    Eigen::Matrix<float, 6, 1> normalization_factors;
    normalization_factors << SCREEN_WIDTH, SCREEN_HEIGHT, BALL_SPEED, BALL_SPEED, SCREEN_HEIGHT, SCREEN_HEIGHT;

    Eigen::Matrix<float, 6, 1> normalized_vars = variables.array() / normalization_factors.array();

    EXPECT_TRUE(normalized_vars.isApprox(expected_normalized)) << "Normalization failed.";
}


TEST_F(DQNTest, BiasExtension)
{
    Eigen::Matrix<float, 6, 1> normalized_vars;
        normalized_vars << 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f;

        Eigen::Matrix<float, 7, 1> expected_extended_vars;
        expected_extended_vars << 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f;

        Eigen::Matrix<float, 7, 1> extended_vars;
        extended_vars << normalized_vars, 1.0f;

        EXPECT_TRUE(extended_vars.isApprox(expected_extended_vars)) << "Bias extension failed.";
}


TEST_F(DQNTest, CorrectResultSize)
{
    agent->W1 = Eigen::MatrixXf::Random(21, 7); // Random 4x7 weights
    agent->W2 = Eigen::MatrixXf::Random(3, 21); // Random 3x4 weights
    
    Eigen::Matrix<float, 6, 1> variables;
    
    variables << 400.0f, 300.0f, 5.0f, 5.0f, 300.0f, 300.0f;
    
    Eigen::Matrix<float, 3, 1> result = DQN(agent, variables);

    // Ensure result is of correct size
    EXPECT_EQ(result.rows(), 3);
    EXPECT_EQ(result.cols(), 1);
}

TEST_F(DQNTest, EndToEnd) {
    agent->W1 = Eigen::MatrixXf::Constant(21, 7, 0.2f);
    agent->W2 = Eigen::MatrixXf::Constant(3, 21, 0.5f); 

    Eigen::Matrix<float, 6, 1> variables;
    variables << 400.0f, 300.0f, 5.0f, 5.0f, 300.0f, 300.0f;

    Eigen::Matrix<float, 3, 1> expected_output;
    expected_output << 1.0f, 1.0f, 1.0f; // Adjust based on exact logic

    Eigen::Matrix<float, 3, 1> result = DQN(agent, variables);
    // std::cout << result << std::endl;
    EXPECT_TRUE(result.isApprox(expected_output, 200.0f)) << "End-to-end computation failed.";
}

TEST_F(DQNTest, RandomEndToEnd) {
    agent->W1 = Eigen::MatrixXf::Random(21, 7);
    agent->W2 = Eigen::MatrixXf::Random(3, 21);

    Eigen::Matrix<float, 6, 1> variables;
    variables << 400.0f, 300.0f, 5.0f, 5.0f, 300.0f, 300.0f;

    Eigen::Matrix<float, 3, 1> expected_output;
    expected_output << 1.0f, 1.0f, 1.0f; // Adjust based on exact logic

    Eigen::Matrix<float, 3, 1> result = DQN(agent, variables);
    // std::cout << result << std::endl;
    EXPECT_TRUE(result.isApprox(expected_output, 200.0f)) << "random end-to-end computation failed.";
    EXPECT_TRUE(result[0] != result[1]) << "random end-to-end computation failed.";

}
