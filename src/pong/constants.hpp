#pragma once

// Constants
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 800;
constexpr int BALL_SPEED = 400;
constexpr int PADDLE_SPEED = 300;
constexpr int PADDLE_WIDTH = 25;
constexpr int PADDLE_HEIGHT = 120;
constexpr int BALL_RADIUS = 20;

    //layerPaddle(SCREEN_WIDTH - PADDLE_WIDTH - 10, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2),
    //cpuPaddle(10, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2),

constexpr int PADDLE_Y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
constexpr int PADDLE1_X = SCREEN_WIDTH - PADDLE_WIDTH - 10;
constexpr int PADDLE2_X = 10;
