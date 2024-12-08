#pragma once
#include "Agent.hpp"
#include "../pong/EpisodeParameter.hpp"
#include "activations.hpp"
#include "../pong/constants.hpp"
#include <memory>
#include <utility>

struct DQNReturn 
{
    Eigen::VectorXf y1;
    Eigen::Matrix<float, 3, 1> v2;
};

Eigen::Matrix<float, 3, 1> DQN(const std::unique_ptr<Agent>& agent, const Eigen::Matrix<float, 6, 1>& variables);

DQNReturn DQN(const std::unique_ptr<Agent>& agent, const Eigen::Matrix<float, 6, 1>& variables, bool intermediatResult);