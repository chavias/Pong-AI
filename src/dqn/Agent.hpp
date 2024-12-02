#pragma once
#include <Eigen/Dense>


class Agent
{
public:
    Eigen::MatrixXd W1;
    Eigen::MatrixXd W2;
    //Eigen::Matrix<double, 21, 7> W1;
    //Eigen::Matrix<double, 7, 3> W2;

public:
    Agent::Agent(size_t L1, size_t numColumnsW1, size_t numRowsW2, size_t numColumnsW2);

    // Copy assignment operator to copy from an Agent
    Agent& operator=(const Agent& agent) {
        W1 = agent.W1; // Copy W1 from Agent
        W2 = agent.W2; // Copy W2 from Agent
        return *this;
    }

    // Overload to copy from an Agent passed as unique_ptr
    Agent& operator=(const std::unique_ptr<Agent>& agent) {
        if (agent) {
            W1 = agent->W1; // Copy W1
            W2 = agent->W2; // Copy W2
        }
        return *this;
    }
private:
    Eigen::MatrixXd generateRandomMatrix(size_t rows, size_t cols, double minVal, double maxVal);
};

struct Target 
{
    Eigen::MatrixXd W1;
    Eigen::MatrixXd W2;

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
