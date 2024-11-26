#include "activations.hpp"

template <typename T>
T activationF(T x)
{
    // relu
    x = std::max(static_cast<T>(0.0f), x);

    // Sigmoid
    x = 1 / (1 + std::exp(-x));

    return x;
}

template <typename T>
T derActivationF(T x)
{
    x = (x > 0) ? 1 : 0;

    x = x * (1 - x);

    return x;
}

// Explicit instantiation for the types you need
template double activationF<double>(double);
template double derActivationF<double>(double);
