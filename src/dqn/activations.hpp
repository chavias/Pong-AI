#pragma once

#include <algorithm>
#include <cmath>
#include <Eigen/Dense>
#include <iostream>

// #define DEBUGActivation

#ifdef DEBUGActivation
#define DEBUGActivation(X) X
#define LOG(X) std::cout << X << std::endl
#endif

#ifndef DEBUGActivation
#define DEBUGActivation(X)
#define LOG(X)
#endif


// template <typename T>
// T derActivationF(const T& x)
// {
//     return (x.array() > 0).template cast<typename T::Scalar>(); // Derivative of ReLU
// }

template <typename T>
auto derActivationF(const T& x) {
    LOG("x before activation derivative : " << x.transpose());
    LOG("x after activation derivative : " << (x.array() > 0).template cast<typename T::Scalar>().matrix().transpose());
    return (x.array() > 0).template cast<typename T::Scalar>().matrix();
}


template <typename T>
T activationF(const T& input)
{
    LOG("x before activation : " << input.transpose());
    LOG("x after activation : " << input.array().max(0).transpose());
    return input.array().max(0); // ReLU example
}