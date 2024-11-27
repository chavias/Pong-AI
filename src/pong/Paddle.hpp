#pragma once
#include "raylib.h"



class Paddle {
protected:
    void LimitMovement();

public:
    float x, y;
    float width, height;
    float speed;

    Paddle(float startX, float startY);
    virtual ~Paddle() = default;

    virtual void Update(float deltaTime, float ballY) = 0;
    void Draw() const;
};

class PlayerPaddle : public Paddle {
public:
    PlayerPaddle(float startX, float startY);
    void Update(float deltaTime, float ballY) override;
};

class CpuPaddle : public Paddle {
public:
    CpuPaddle(float startX, float startY);
    void Update(float deltaTime, float ballY) override;
};
