#include <Agent.hpp>


Agent MiniBatchSGD(Agent& agent, Target& target, float learningRate, float discount, int miniBatchSize, int Regularization)
{
    Eigen::Matrix<double, 21, 7> dW1; //  derivative W1
    Eigen::Matrix<double, 7, 3> dW2;  //  

    MemoryElement mem;
    for (size_t i = 0; i<miniBatchSize; i++)
    {
        mem.sample();
    }


}