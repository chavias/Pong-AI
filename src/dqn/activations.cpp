#include "activations.hpp"

// Explicit instantiations
template Eigen::Vector3f activationF(const Eigen::Vector3f& input);
template Eigen::Vector3f derActivationF(const Eigen::Vector3f& x);
