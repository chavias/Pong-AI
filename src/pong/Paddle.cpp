#include "constants.hpp"
#include "Paddle.hpp"

Paddle::Paddle(float startX, float startY)
    : x(startX), y(startY), width(PADDLE_WIDTH), height(PADDLE_HEIGHT), speed(PADDLE_SPEED) {}

void Paddle::LimitMovement()
{
    if (y <= 0)
    {
        y = 0;
    }
    if (y + height >= SCREEN_HEIGHT)
    {
        y = SCREEN_HEIGHT - height;
    }
}

void Paddle::Reset()
{
    y = PADDLE_Y;
}

void Paddle::Draw() const
{
    DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
}

PlayerPaddle::PlayerPaddle(float startX, float startY) : Paddle(startX, startY) {}

void PlayerPaddle::Update(float deltaTime, float ballY, Action act)
{
    if (IsKeyDown(KEY_UP))
    {
        y -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        y += speed * deltaTime;
    }
    LimitMovement();
}

CpuPaddle::CpuPaddle(float startX, float startY) : Paddle(startX, startY) {}

void CpuPaddle::Update(float deltaTime, float ballY, Action act)
{
    if (y + height / 2 > ballY)
    {
        y -= speed * deltaTime;
    }
    if (y + height / 2 < ballY)
    {
        y += speed * deltaTime;
    }
    LimitMovement();
}

AIPaddle::AIPaddle(float startX, float startY) : Paddle(startX, startY) {}

void AIPaddle::Update(float deltaTime, float ballY, Action action)
{
    if (action == DOWN)
    {
        y -= speed * deltaTime;
    }
    if (action == UP)
    {
        y += speed * deltaTime;
    }
    LimitMovement();
}


WallPaddle::WallPaddle(float startX, float startY)
    : Paddle(startX, 0)
{
    y = 0;
    // Set dimensions to cover the right side of the screen
    height = SCREEN_HEIGHT;       // Full screen height
    width = PADDLE_WIDTH;         // Keep the width as defined
    x = SCREEN_WIDTH - width;     // Align paddle to the right edge
}


void WallPaddle::Update(float deltaTime, float ballY, Action action)
{
    if (y + height / 2 > ballY)
    {
        y -= speed * deltaTime;
    }
    if (y + height / 2 < ballY)
    {
        y += speed * deltaTime;
    }
    LimitMovement();
}