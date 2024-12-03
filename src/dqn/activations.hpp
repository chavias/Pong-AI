#pragma once

#include <algorithm>
#include <cmath>
#include <Eigen/Dense>

// template <typename T>
// T derActivationF(const T& x)
// {
//     return (x.array() > 0).template cast<typename T::Scalar>(); // Derivative of ReLU
// }

template <typename T>
auto derActivationF(const T& x) {
    return (x.array() > 0).template cast<typename T::Scalar>().matrix();
}


template <typename T>
T activationF(const T& input)
{
    return input.array().max(0); // ReLU example
}