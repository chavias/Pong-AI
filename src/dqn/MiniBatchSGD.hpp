#include "Agent.hpp"
#include "Memory.hpp"
#include <memory>
#include "../pong/EpisodeParameter.hpp"



Agent MiniBatchSGD(Agent& agent, Target& target, float learningRate, float discount, int miniBatchSize, int Regularization)
{
    Eigen::Matrix<double, 21, 7> dW1; //  derivative W1
    Eigen::Matrix<double, 7, 3> dW2;  //  

    std::unique_ptr<Memory> mem;
    for (size_t i = 0; i<miniBatchSize; i++)
    {
        EpisodeParameter mem->sample();

        dW1temp Gradient()
        dW1temp Gradient()

    }

};