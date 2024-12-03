#pragma once
#include "Agent.hpp"
#include "EpisodeParameter.hpp"
#include "activations.hpp"
#include "constants.hpp"
#include <memory>
#include <utility>


Eigen::Matrix<float, 3, 1> DQN(const std::unique_ptr<Agent>& agent, const Eigen::Matrix<float, 6, 1>& variables);