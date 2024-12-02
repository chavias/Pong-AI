#pragma once
#include "Ball.hpp"
#include "Paddle.hpp"
#include "Action.hpp"
#include "raylib.h"
#include "ScoreManager.hpp"
#include "constants.hpp"
#include <array>
#include <memory>
#include "EpisodeParameter.hpp"


class Game
{
private:
    std::unique_ptr<Ball> ball;
    std::unique_ptr<Paddle> paddle1;
    std::unique_ptr<Paddle> paddle2;
    std::unique_ptr<ScoreManager> scoreManager;

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
    EpisodeParameter Step(float deltaTime, Action action1, Action action2);


private:
    // can be replaced with Step
    void Update(float deltaTime);

    void Render() const;

    void Reset();
};
