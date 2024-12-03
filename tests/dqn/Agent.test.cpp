#include <gtest/gtest.h>
#include "../../src/dqn/Agent.hpp"
#include <Eigen/Dense>

// Test for the generateRandomMatrix function
TEST(AgentTest, GenerateRandomMatrix_Size) {
    Agent agent(21, 7, 3, 21);  // Example Agent with specific matrix sizes
    Eigen::MatrixXf matrix = agent.generateRandomMatrix(21, 7, -1.0, 1.0);

    // Test the size of the generated matrix
    EXPECT_EQ(matrix.rows(), 21);
    EXPECT_EQ(matrix.cols(), 7);
}

TEST(AgentTest, GenerateRandomMatrix_ValueRange) {
    Agent agent(21, 7, 3, 21);
    Eigen::MatrixXf matrix = agent.generateRandomMatrix(21, 7, -1.0, 1.0);

    // Test that all values in the matrix are within the range [-1.0, 1.0]
    for (int i = 0; i < matrix.rows(); ++i) {
        for (int j = 0; j < matrix.cols(); ++j) {
            EXPECT_GE(matrix(i, j), -1.0);
            EXPECT_LE(matrix(i, j), 1.0);
        }
    }
}

// Test for the Agent constructor
TEST(AgentTest, Constructor_InitializesMatrices) {
    // Creating an Agent with specific dimensions for W1 and W2
    Agent agent(21, 7, 3, 21);
    
    // Check that the matrices are not empty
    EXPECT_GT(agent.W1.rows(), 0);
    EXPECT_GT(agent.W1.cols(), 0);
    EXPECT_GT(agent.W2.rows(), 0);
    EXPECT_GT(agent.W2.cols(), 0);

    // Check the size of W1 and W2 matches the constructor parameters
    EXPECT_EQ(agent.W1.rows(), 21);
    EXPECT_EQ(agent.W1.cols(), 7);
    EXPECT_EQ(agent.W2.rows(), 3);
    EXPECT_EQ(agent.W2.cols(), 21);
}
