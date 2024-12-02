#pragma once
#include <random>
#include "Action.hpp"

struct Random
{
private:
    std::random_device rd; // Seed source
    std::mt19937 gen; // Mersenne Twister generator
    std::uniform_int_distribution<int> distAction{1, 3}; // For actions (1 to 3)
    std::uniform_real_distribution<double> distEpsilon{0.0, 1.0}; // For epsilon (0.0 to 1.0)

public:
    Random() : gen(rd()) {} // Initialize the random generator once with the seed

    Action randomAction()
    {
        return static_cast<Action>(distAction(gen)); // Generate random action as Action enum
    }

    float randomEpsilon()
    {
        return distEpsilon(gen); // Return random double for epsilon
    }
};



// tint main()
// {
//     Random random;

//     Action action = random.randomAction();
//     float epsilon = random.randomEpsilon();
// }