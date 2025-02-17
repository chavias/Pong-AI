#pragma once
#include <random>
#include "../pong/Action.hpp"
#include <iostream>

struct Rand
{
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> distAction{0, 2};
    std::uniform_real_distribution<double> distEpsilon{0.0, 1.0};

public:
    Rand() : gen(rd()) {}

    Action randomAction()
    {
        return static_cast<Action>(distAction(gen));
    }

    float randomEpsilon()
    {
        return distEpsilon(gen);
    }
};



// int main()
// {
//     Random random;

//     Action action = random.randomAction();
//     float epsilon = random.randomEpsilon();
// }