#include "Agent.hpp"
#include <random>

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
Agent::Agent(size_t L1, size_t numColumnsW1, size_t numRowsW2, size_t numColumnsW2) {
        W1 = generateRandomMatrix(L1, numColumnsW1, -1.0, 1.0);
        W2 = generateRandomMatrix(numRowsW2, numColumnsW2, -1.0, 1.0);
    }