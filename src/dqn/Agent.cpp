#include "Agent.hpp"
#include <random>
#include <iostream>

Eigen::MatrixXf Agent::generateRandomMatrix(size_t rows, size_t cols, float minVal, float maxVal)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(minVal, maxVal);

    // Initialize the matrix and populate it with random values
    Eigen::MatrixXf matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            matrix(i, j) = dis(gen);
        }
    }
    return matrix;
}

// Constructor to initialize matrices
Agent::Agent(size_t L1, size_t numColumnsW1, size_t numRowsW2, size_t numColumnsW2)
{
        W1 = generateRandomMatrix(L1, numColumnsW1, 0.0, 1.0);
        W2 = generateRandomMatrix(numRowsW2, numColumnsW2, 0.0, 1.0);
}

// Copy Constructor
Agent::Agent(const Agent& other) 
{
    W1 = other.W1;
    W2 = other.W2;
}

// Copy assignment operator to copy from an Agent
Agent& Agent::operator=(const Agent& agent)
{
    W1 = agent.W1; // Copy W1 from Agent
    W2 = agent.W2; // Copy W2 from Agent
    return *this;
}

// Overload to copy from an Agent passed as unique_ptr
Agent& Agent::operator=(const std::unique_ptr<Agent>& agent) {
    if (agent) {
        W1 = agent->W1; // Copy W1
        W2 = agent->W2; // Copy W2
    }
    return *this;
}