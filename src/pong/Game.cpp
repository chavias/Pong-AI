#include "Game.hpp"
#include "constants.hpp"
#include <iostream>

Game::Game()
    : playerPaddle(SCREEN_WIDTH - PADDLE_WIDTH - 10, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2),
      cpuPaddle(10, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2),
      resetTimer(0.0f),
      isBallFrozen(false)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);
}


Game::~Game()
{
    CloseWindow();
}

void Game::Run()
{
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        // Update(deltaTime);
        EpisodeParameter state = Step(deltaTime);
        // std::cout << state.reward1 << ' ' << state.reward2 << "\n";
        Render();
    }
}

/// @brief Single step of the game 
/// @param deltaTime 
/// @return state and rewards
EpisodeParameter Game::Step(float deltaTime)
{
    playerPaddle.Update(deltaTime);
    cpuPaddle.UpdateWithBall(deltaTime, ball.y);

    if (isBallFrozen)
    {
        // Update the reset timer
        resetTimer += deltaTime;
        if (resetTimer >= 1.0f)
        {
            isBallFrozen = false;
            resetTimer = 0.0f; // Reset the timer
            ball.StartMoving();
        }
    }
    else
    {
        ball.Update(deltaTime);
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                    Rectangle{playerPaddle.x, playerPaddle.y, playerPaddle.width, playerPaddle.height}))
        {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                    Rectangle{cpuPaddle.x, cpuPaddle.y, cpuPaddle.width, cpuPaddle.height}))
        {
            ball.speed_x *= -1;
        }

        // count score and reset
        if (ball.x - ball.radius <= 0)
        {
            scoreManager.PlayerScored();

            reward1+=1; // increase reward 

            ball.Reset();
            isBallFrozen = true;
        }
        else if (ball.x + ball.radius >= SCREEN_WIDTH)
        {
            scoreManager.CpuScored();
            
            reward2+=1; // increase reward 

            ball.Reset();
            isBallFrozen = true;
        }
    }
    return {ball.x, ball.y, ball.speed_x, ball.speed_y, cpuPaddle.y,
            reward1, reward2, false};
}

void Game::Update(float deltaTime)
{
    playerPaddle.Update(deltaTime);
    cpuPaddle.UpdateWithBall(deltaTime, ball.y);

    if (isBallFrozen)
    {
        // Update the reset timer
        resetTimer += deltaTime;
        if (resetTimer >= 1.0f)
        {
            isBallFrozen = false;
            resetTimer = 0.0f; // Reset the timer
            ball.StartMoving();
        }
    }
    else
    {
        ball.Update(deltaTime);
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                    Rectangle{playerPaddle.x, playerPaddle.y, playerPaddle.width, playerPaddle.height}))
        {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                    Rectangle{cpuPaddle.x, cpuPaddle.y, cpuPaddle.width, cpuPaddle.height}))
        {
            ball.speed_x *= -1;
        }

        // count score and reset
        if (ball.x - ball.radius <= 0)
        {
            scoreManager.PlayerScored();
            ball.Reset();
            isBallFrozen = true;
        }
        else if (ball.x + ball.radius >= SCREEN_WIDTH)
        {
            scoreManager.CpuScored();
            ball.Reset();
            isBallFrozen = true;
        }
    }
}

void Game::Render() const
{
    BeginDrawing();
    ClearBackground(DARKGREEN);

    DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, DARKGREEN);
    // DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150, LIME);
    DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);

    ball.Draw();
    playerPaddle.Draw();
    cpuPaddle.Draw();
    scoreManager.DrawScore();

    EndDrawing();
}