#include "Game.hpp"
#include "constants.hpp"

Game::Game() 
    : playerPaddle(SCREEN_WIDTH - PADDLE_WIDTH - 10, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2),
      cpuPaddle(10, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);
}

Game::~Game() {
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        Update(deltaTime);
        Render();
    }
}

void Game::Update(float deltaTime) {
    ball.Update(deltaTime);
    playerPaddle.Update(deltaTime);
    cpuPaddle.UpdateWithBall(deltaTime, ball.y);

    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, 
                                Rectangle{playerPaddle.x, playerPaddle.y, playerPaddle.width, playerPaddle.height})) {
        ball.speed_x *= -1;
    }

    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, 
                                Rectangle{cpuPaddle.x, cpuPaddle.y, cpuPaddle.width, cpuPaddle.height})) {
        ball.speed_x *= -1;
    }

    // count score and reset
    if (ball.x - ball.radius <= 0) {
        scoreManager.PlayerScored();
        ball.Reset();
    } else if (ball.x + ball.radius >= SCREEN_WIDTH) {
        scoreManager.CpuScored();
        ball.Reset();
    }
}

void Game::Render() const {
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
