#pragma once
#include <Eigen/Dense>
#include "Paddle.hpp"
#include "Action.hpp"

struct EpisodeParameter
{   
    EpisodeParameter() 
        : pongVariables(Eigen::Matrix<float, 6, 1>::Zero()), 
          action1(WAIT),
          action2(WAIT),
          reward1(0),
          reward2(0),
          gameEnd(false)
    {};

    EpisodeParameter(Eigen::Matrix<float, 6, 1> variables,
                    Action act1,
                    Action act2,
                    int rew1,
                    int rew2,
                    bool gameEnd)
        : pongVariables(std::move(variables)), 
          action1(act1),
          action2(act2),
          reward1(0),
          reward2(0),
          gameEnd(gameEnd)
    {};

    Eigen::Matrix<float, 6, 1> pongVariables;
    //  ball_x, ball_y, ball_vx, ball_vy, paddle1 paddle2;
    
    // y coordinate of the center of the padddle
    Action action1 = WAIT;
    Action action2 = WAIT;

    // Q2 has to be added maybe 
    int reward1 = 0;
    int reward2 = 0;
    bool gameEnd = false;
};