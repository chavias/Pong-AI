#pragma once
#include <Eigen/Dense>
#include "Paddle.hpp"
#include "Action.hpp"

struct EpisodeParameter
{   
    // EpisodeParameter() 
    //     : pongVariables(Eigen::Matrix<float, 6, 1>::Zero()), 
    //       action1(WAIT),
    //       action2(WAIT),
    //       reward1(0),
    //       reward2(0),
    //       gameEnd(false)
    // {}

    // Initialize other members here if needed
    // pong variables could be initialist with 
    Eigen::Matrix<float, 6, 1> pongVariables;
    //     float ball_x;
    //     float ball_y;

    //     float ball_vx;
    //     float ball_vy; 

    //     float paddle1;
    //     float paddle2;
    
    // y coordinate of the center of the padddle
    Action action1 = WAIT;
    Action action2 = WAIT;

    // Q2 has to be added maybe 
    int reward1 = 0;
    int reward2 = 0;
    bool gameEnd = false;
};