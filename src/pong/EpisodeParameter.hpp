#pragma once
#include <Eigen/Dense>
#include "Paddle.hpp"



struct PongVariables
{
    float ball_x;
    float ball_y;

    float ball_vx;
    float ball_vy; 

    float paddle1;
    float paddle2; 
};


struct EpisodeParameter
{
    Eigen::Matrix<float, 6, 1> pongVariables;
    // float ball_x;
    // float ball_y;

    // float ball_vx;
    // float ball_vy;

    // y coordinate of the center of the padddle
    Action action1 = WAIT;
    Action action2 = WAIT;

    // Q2 has to be added

    int reward1 = 0;
    int reward2 = 0;
    bool gameEnd = false;

};