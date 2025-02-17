#include "Agent.hpp"
#include <random>
#include <iostream>
#include <fstream>
#include <stdexcept>

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
        W1 = generateRandomMatrix(L1, numColumnsW1, -1, 1); // 1
        W2 = generateRandomMatrix(numRowsW2, numColumnsW2, -1, 1); // 1
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

// Save the agent to a file
void Agent::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        throw std::ios_base::failure("Failed to open file for saving: " + filename);
    }

    // Save dimensions of W1
    size_t rowsW1 = W1.rows(), colsW1 = W1.cols();
    outFile.write(reinterpret_cast<const char*>(&rowsW1), sizeof(rowsW1));
    outFile.write(reinterpret_cast<const char*>(&colsW1), sizeof(colsW1));

    // Save data of W1
    outFile.write(reinterpret_cast<const char*>(W1.data()), rowsW1 * colsW1 * sizeof(float));

    // Save dimensions of W2
    size_t rowsW2 = W2.rows(), colsW2 = W2.cols();
    outFile.write(reinterpret_cast<const char*>(&rowsW2), sizeof(rowsW2));
    outFile.write(reinterpret_cast<const char*>(&colsW2), sizeof(colsW2));

    // Save data of W2
    outFile.write(reinterpret_cast<const char*>(W2.data()), rowsW2 * colsW2 * sizeof(float));

    outFile.close();
}

// Load the agent from a file
void Agent::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        throw std::ios_base::failure("Failed to open file for loading: " + filename);
    }

    // Load dimensions of W1
    size_t rowsW1, colsW1;
    inFile.read(reinterpret_cast<char*>(&rowsW1), sizeof(rowsW1));
    inFile.read(reinterpret_cast<char*>(&colsW1), sizeof(colsW1));

    // Resize W1 and load its data
    W1.resize(rowsW1, colsW1);
    inFile.read(reinterpret_cast<char*>(W1.data()), rowsW1 * colsW1 * sizeof(float));

    // Load dimensions of W2
    size_t rowsW2, colsW2;
    inFile.read(reinterpret_cast<char*>(&rowsW2), sizeof(rowsW2));
    inFile.read(reinterpret_cast<char*>(&colsW2), sizeof(colsW2));

    // Resize W2 and load its data
    W2.resize(rowsW2, colsW2);
    inFile.read(reinterpret_cast<char*>(W2.data()), rowsW2 * colsW2 * sizeof(float));

    inFile.close();
}


void Agent::softUpdate(std::unique_ptr<Agent>& currentAgent, float tau)
{
    // std::cout << "[+] performed soft update of agent" << std::endl;

    W1 = tau*currentAgent->W1 + (1.0 - tau) * W1;
    W2 = tau*currentAgent->W2 + (1.0 - tau) * W2;
}