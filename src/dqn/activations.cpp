#include "activations.hpp"

template <typename T>
T derActivationF(const T& x)
{
    return (x.array() > 0).template cast<typename T::Scalar>(); // Derivative of ReLU (1 for x > 0, 0 otherwise)
}

template <typename T>
T activationF(const T& input)
{
    return input.array().max(0); // ReLU example
}

// Explicit instantiations
template Eigen::Vector3f activationF(const Eigen::Vector3f& input);
template Eigen::Vector3f derActivationF(const Eigen::Vector3f& x);
