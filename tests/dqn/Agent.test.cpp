#include <gtest/gtest.h>
#include "../../src/dqn/Agent.hpp"
#include <Eigen/Dense>

// Test for the generateRandomMatrix function
TEST(AgentTest, GenerateRandomMatrix_Size) {
    Agent agent(3, 4, 2, 5);  // Example Agent with specific matrix sizes
    Eigen::MatrixXd matrix = agent.generateRandomMatrix(3, 4, -1.0, 1.0);

    // Test the size of the generated matrix
    EXPECT_EQ(matrix.rows(), 3);
    EXPECT_EQ(matrix.cols(), 4);
}

TEST(AgentTest, GenerateRandomMatrix_ValueRange) {
    Agent agent(3, 4, 2, 5);
    Eigen::MatrixXd matrix = agent.generateRandomMatrix(3, 4, -1.0, 1.0);

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
    Agent agent(3, 4, 2, 5);
    
    // Check that the matrices are not empty
    EXPECT_GT(agent.W1.rows(), 0);
    EXPECT_GT(agent.W1.cols(), 0);
    EXPECT_GT(agent.W2.rows(), 0);
    EXPECT_GT(agent.W2.cols(), 0);

    // Check the size of W1 and W2 matches the constructor parameters
    EXPECT_EQ(agent.W1.rows(), 3);
    EXPECT_EQ(agent.W1.cols(), 4);
    EXPECT_EQ(agent.W2.rows(), 2);
    EXPECT_EQ(agent.W2.cols(), 5);
}
