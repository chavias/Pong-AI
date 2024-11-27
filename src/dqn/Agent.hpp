#pragma once
#include <Eigen/Dense>


struct MemoryElement
{
    int reward1 = 0;
    int reward2 = 0; 

    int action1 = 0;
    int action2 = 0;

    bool endGame = 0;

    MemoryElement sample(); // get random sample from the memory array
};



struct Agent
{
    Eigen::Matrix<double, 21, 7> W1;
    Eigen::Matrix<double, 7, 3> W2;

};

struct Target 
{
    Eigen::Matrix<double, 21, 7> W1;
    Eigen::Matrix<double, 7, 3> W2;

};
