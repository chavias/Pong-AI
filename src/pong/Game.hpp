#pragma once
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Action.hpp"
#include "ScoreManager.hpp"
#include "constants.hpp"
#include "EpisodeParameter.hpp"
#include "raylib.h"
#include <array>
#include <memory>


class Game
{
public:
    explicit Game(std::unique_ptr<Paddle>&& p1 = {}, std::unique_ptr<Paddle>&& p2 = {});
 
    ~Game();

    void Run();

    void set_paddles(std::unique_ptr<Paddle>&& p1, std::unique_ptr<Paddle>&& p2)
    {
        paddle1 = std::move(p1);
        paddle2 = std::move(p2);
    }

    EpisodeParameter Step(float deltaTime, Action action1, Action action2);

    void set_player(bool side)
    {
        if (side)
        {
            paddle1 = std::make_unique<PlayerPaddle>(PADDLE1_X, PADDLE_Y);
        }
        else 
        {
            paddle2 = std::make_unique<PlayerPaddle>(PADDLE2_X, PADDLE_Y);
        }
    };
    
    void Render() const;

    std::unique_ptr<ScoreManager> scoreManager;
    
private:
    std::unique_ptr<Ball> ball;
    std::unique_ptr<Paddle> paddle1;
    std::unique_ptr<Paddle> paddle2;

private:


    EpisodeParameter Reset();
};
