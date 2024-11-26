#pragma once
#include "raylib.h"

class Ball {
public:
    float x, y;
    float speed_x, speed_y;
    int radius;

    Ball();
    void StartMoving();
    void Reset();
    void Update(float deltaTime);
    void Draw() const;
};
