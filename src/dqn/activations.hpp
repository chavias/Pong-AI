#pragma once

#include <algorithm>
#include <cmath>
#include <Eigen/Dense>


template <typename T>
T derActivationF(const T& x);

template <typename T>
T activationF(const T& input);


// Explicit instantiation for Eigen::Vector3f
extern template Eigen::Vector3f activationF(const Eigen::Vector3f& input);
extern template Eigen::Vector3f derActivationF(const Eigen::Vector3f& x);
