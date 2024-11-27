#pragma once
#include "Ball.hpp"
#include "Paddle.hpp"
#include "raylib.h"
#include "ScoreManager.hpp"
#include <array>

struct EpisodeParameter
{
    float ball_x;
    float ball_y;

    float ball_vx;
    float ball_vy;

    float paddle_coordinate; // y coordinate of the center of the padddle
    int reward1 = 0;
    int reward2 = 0;
    bool gameEnd;

};

class Game
{
private:
    Ball ball;
    PlayerPaddle playerPaddle;
    CpuPaddle cpuPaddle;
    ScoreManager scoreManager;

    // seperate rewards might be unneccessary if reward = score
    int reward1 = 0; 
    int reward2 = 0;

    float resetTimer;
    bool isBallFrozen;

public:
    Game();
    ~Game();
    void Run();


public:  
    EpisodeParameter Step(float deltaTime);


private:
    // can be replaced with Step
    void Update(float deltaTime);

    void Render() const;
};
