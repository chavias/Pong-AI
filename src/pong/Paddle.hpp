#pragma once
#include "raylib.h"
#include "Action.hpp"

// Base class for the strategy design pattern
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

    void Reset();

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
};