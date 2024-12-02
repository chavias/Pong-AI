#include "Game.hpp"
#include "constants.hpp"
#include <iostream>

Game::Game(std::unique_ptr<Paddle> &&p1, std::unique_ptr<Paddle> &&p2) 
    : paddle1(std::move(p1)),
      paddle2(std::move(p2)),
      resetTimer(0.0f), isBallFrozen(false),
      ball(std::make_unique<Ball>()),
      scoreManager(std::make_unique<ScoreManager>())
{
    if (!paddle1) {
    paddle1 = std::make_unique<CpuPaddle>(PADDLE1_X, PADDLE_Y);
    }
    if (!paddle2) {
        paddle2 = std::make_unique<PlayerPaddle>(PADDLE2_X, PADDLE_Y);
    }

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
        Action action1 = WAIT;
        Action action2 = WAIT;
        EpisodeParameter state = Step(deltaTime, action1, action2);
        std::cout << state.reward1 << state.reward2 << '\n';
        Render();
    }
}

/// @brief Provides State and Rewards for the Agent to learn 
/// @param deltaTime, action1, action2,
/// @return EpisodeParameter structure represents current reward and game state 
EpisodeParameter Game::Step(float deltaTime, Action action1, Action action2)
{
    paddle1->Update(deltaTime, ball->y, action1);
    paddle2->Update(deltaTime, ball->y, action2);

    if (isBallFrozen)
    {
        // Update the reset timer
        resetTimer += deltaTime;
        if (resetTimer >= 1.0f)
        {
            isBallFrozen = false;
            resetTimer = 0.0f; // Reset the timer
            ball->StartMoving();
        }
    }
    else
    {
        ball->Update(deltaTime);
        if (CheckCollisionCircleRec(Vector2{ball->x, ball->y}, ball->radius,
                                    Rectangle{paddle1->x, paddle1->y, paddle1->width, paddle1->height}))
        {
            ball->speed_x *= -1;
            reward1 += 1; // increase reward
        }

        if (CheckCollisionCircleRec(Vector2{ball->x, ball->y}, ball->radius,
                                    Rectangle{paddle2->x, paddle2->y, paddle2->width, paddle2->height}))
        {
            ball->speed_x *= -1;
            reward2 += 1; // increase reward
        }

        // count score and reset
        if (ball->x - ball->radius <= 0)
        {
            scoreManager->PlayerScored();

            reward1 -= 1; // decrease reward 
            gameEnd = true;
            ball->Reset();
            isBallFrozen = true;
        }
        else if (ball->x + ball->radius >= SCREEN_WIDTH)
        {
            scoreManager->CpuScored();
            
            reward2 -= 1; // decrease reward 
            gameEnd = true;
            ball->Reset();
            
            isBallFrozen = true;
        }
    }
    Eigen::Matrix<float, 6, 1> pongVariables(ball->x, ball->y, ball->speed_x, ball->speed_y, paddle1->y, paddle2->y);
    return {pongVariables, action1, action2, reward1, reward2, gameEnd};
}


void Game::Render() const
{
    BeginDrawing();
    ClearBackground(DARKGREEN);

    DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, DARKGREEN);
    // DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150, LIME);
    DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);

    ball->Draw();
    paddle1->Draw();
    paddle2->Draw();
    scoreManager->DrawScore();

    EndDrawing();
}