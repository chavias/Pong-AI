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
    std::unique_ptr<Agent> agent = std::make_unique<Agent>(21, 7, 3, );
    
    void SetUp() override
    {
        memory = std::make_unique<Memory>();  // Initialize the unique_ptr
    }

    // Add mock data to memory for the tests
    void fillMemory() {
        for (size_t i = 0; i < memory_capacity; ++i) {
            EpisodeParameter ep;
            ep.pongVariables.setZero();  // Initialize pongVariables
            ep.action1 = static_cast<Action>(i % 3);  // Cycle through actions
            ep.reward1 = static_cast<int>(i);  // Use the index as reward
            ep.reward2 = static_cast<int>(i + 1);  // Use a different value for reward2
            ep.gameEnd = (i == memory_capacity - 1);  // Mark the last entry as game end
            memory->append(ep);
        }
    }

        // Add mock data to memory for the tests
    void fillAllMemory() {
        for (size_t i = 0; i < 100000; ++i) {
            EpisodeParameter ep;
            ep.pongVariables.setZero();  // Initialize pongVariables
            ep.action1 = static_cast<Action>(i % 3);  // Cycle through actions
            ep.reward1 = static_cast<int>(i);  // Use the index as reward
            ep.reward2 = static_cast<int>(-i);  // Use a different value for reward2
            ep.gameEnd = (i == memory_capacity - 1);  // Mark the last entry as game end
            memory->append(ep);
        }
    }
};


Eigen::Matrix<float, 3, 1> DQN(const std::unique_ptr<Agent>& agent, const Eigen::Matrix<float, 6, 1>& variables)
{
    // Constants for normalization
    Eigen::Matrix<float, 6, 1> normalization_factors;
    normalization_factors << SCREEN_WIDTH, SCREEN_HEIGHT, BALL_SPEED, BALL_SPEED, SCREEN_HEIGHT, SCREEN_HEIGHT;

    // Normalize Pong Variables
    Eigen::Matrix<float, 6, 1> normalized_vars = variables.array() / normalization_factors.array();

    // Extend normalized_vars with a constant 1.0
    Eigen::Matrix<float, 7, 1> extended_vars;
    extended_vars << normalized_vars, 1.0f;

    // Layer 1: Weighted sum (+ bias)
    Eigen::MatrixXf v1 = (agent->W1 * extended_vars).array();
    Eigen::MatrixXf y1 = activationF(v1); // Apply activation function

    // Layer 2: Weighted sum (output layer)
    Eigen::MatrixXf v2 = (agent->W2 * y1).array();

    return v2; // size (3, 1)
}
