#pragma once
#include "raylib.h"

/**
 * Implementation of the Strategy pattern for the Paddles
 */

// Enum for the 3 actions WAIT, UP, DOWN
enum Action
{
    WAIT = 0,
    UP,
    DOWN
};

class Paddle
{
protected:
    void LimitMovement();

public:
    float x, y;
    float width, height;
    float speed;

    Paddle(float startX, float startY);
    virtual ~Paddle() = default;

    virtual void Update(float deltaTime, float ballY, Action action) = 0;
    void Draw() const;
};

class PlayerPaddle : public Paddle
{
public:
    PlayerPaddle(float startX, float startY);
    void Update(float deltaTime, float ballY, Action action) override;
};

class CpuPaddle : public Paddle
{
public:
    CpuPaddle(float startX, float startY);
    void Update(float deltaTime, float ball, Action action) override;
};

class AIPaddle : public Paddle
{
// public:
//     Action action = WAIT;

public:
    AIPaddle(float startX, float startY);
    void Update(float deltaTime, float ballY, Action action) override;

    void setAction(Action act);
};