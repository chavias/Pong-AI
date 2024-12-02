#pragma once
#include "Agent.hpp"
#include "EpisodeParameter.hpp"
#include "activations.hpp"
#include "constants.hpp"
#include <memory>
#include <utility>


Eigen::Matrix<float, 3, 1> DQN(const std::unique_ptr<Agent>& agent, const Eigen::Matrix<float, 6, 1>& variables)
{
    // Constants for normalization
    Eigen::Matrix<float, 6, 1> normalization_factors;
    normalization_factors << SCREEN_WIDTH, SCREEN_HEIGHT, BALL_SPEED, BALL_SPEED, SCREEN_HEIGHT, SCREEN_HEIGHT;

    // Normalize Pong Variables
    Eigen::Matrix<float, 6, 1> normalized_vars = variables.array() / normalization_factors.array();

    // Layer 1: Weighted sum (+ bias)
    Eigen::MatrixXf v1 = (agent->W1 * normalized_vars).array(); //+ agent->b1.array();
    Eigen::MatrixXf y1 = activationF(v1); // Apply activation function

    // Layer 2: Weighted sum (output layer)
    Eigen::MatrixXf v2 = (agent->W2 * y1).array(); //+ agent->b2.array();

    // Assuming v2 is a scalar
    return v2; // Extract the first element (float)
}
