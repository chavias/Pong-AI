#pragma once
#include "Ball.hpp"
#include "Paddle.hpp"
#include "raylib.h"
#include "ScoreManager.hpp"
#include "constants.hpp"
#include <array>
#include <memory>


struct EpisodeParameter
{
    float ball_x;
    float ball_y;

    float ball_vx;
    float ball_vy;

    // y coordinate of the center of the padddle
    float paddle1;
    float paddle2; 
    int reward1 = 0;
    int reward2 = 0;
    bool gameEnd;

};

class Game
{
private:
    std::unique_ptr<Ball> ball;
    std::unique_ptr<Paddle> paddle1;
    std::unique_ptr<Paddle> paddle2;
    std::unique_ptr<ScoreManager> scoreManager;

    // seperate rewards might be unneccessary if reward = score
    int reward1 = 0; 
    int reward2 = 0;

    float resetTimer;
    bool isBallFrozen;

public:
    explicit Game(std::unique_ptr<Paddle>&& p1 = {}, std::unique_ptr<Paddle>&& p2 = {});
    ~Game();
    void Run();

    void set_paddles(std::unique_ptr<Paddle>&& p1, std::unique_ptr<Paddle>&& p2)
    {
        paddle1 = std::move(p1);
        paddle2 = std::move(p2);
    }

public:  
    EpisodeParameter Step(float deltaTime);


private:
    // can be replaced with Step
    void Update(float deltaTime);

    void Render() const;
};
