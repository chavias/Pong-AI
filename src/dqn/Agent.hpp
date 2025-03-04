#pragma once
#include <Eigen/Dense>
#include <memory>


class Agent
{
public:
    Eigen::MatrixXf W1;
    Eigen::MatrixXf W2;
    
    // Save agent to a file
    void saveToFile(const std::string& filename) const;

    // Load agent from a file
    void loadFromFile(const std::string& filename);

    
    // 21, 7, 3, 21
    Agent(size_t L1, size_t numColumnsW1, size_t numRowsW2, size_t numColumnsW2);

    Agent(const Agent& other); 

    // Copy assignment operator to copy from an Agent
    Agent& operator=(const Agent& agent);

    // Overload to copy from an Agent passed as unique_ptr
    Agent& operator=(const std::unique_ptr<Agent>& agent);

    // private:
    Eigen::MatrixXf generateRandomMatrix(size_t rows, size_t cols, float minVal, float maxVal);


    void softUpdate(std::unique_ptr<Agent>& currentAgent, float tau);
};

struct Target 
{
    Eigen::MatrixXf W1;
    Eigen::MatrixXf W2;

    // Default constructor
    Target() = default;

    // Copy assignment operator to copy from an Agent
    Target& operator=(const Agent& agent) {
        W1 = agent.W1; // Copy W1 from Agent
        W2 = agent.W2; // Copy W2 from Agent
        return *this;
    }

    // Overload to copy from an Agent passed as unique_ptr
    Target& operator=(const std::unique_ptr<Agent>& agent) {
        if (agent) {
            W1 = agent->W1; // Copy W1
            W2 = agent->W2; // Copy W2
        }
        return *this;
    }
};
