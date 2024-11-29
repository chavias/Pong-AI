#pragma once
#include <Eigen/Dense>


class Agent
{
public:
    Eigen::MatrixXd W1;
    Eigen::MatrixXd W2;
    //Eigen::Matrix<double, 21, 7> W1;
    //Eigen::Matrix<double, 7, 3> W2;

public:
    Agent::Agent(size_t L1, size_t numColumnsW1, size_t numRowsW2, size_t numColumnsW2);

private:
    Eigen::MatrixXd generateRandomMatrix(size_t rows, size_t cols, double minVal, double maxVal);
};

struct Target 
{
    Eigen::Matrix<double, 21, 7> W1;
    Eigen::Matrix<double, 7, 3> W2;

};
