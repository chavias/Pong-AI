#include "DQN.hpp"

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
