#pragma once

// Constants
constexpr int SCREEN_WIDTH = 40;  //80/4;////300/2; // 1280
constexpr int SCREEN_HEIGHT = 20; ///4; // 800
constexpr int BALL_SPEED = 6; //400;
constexpr int PADDLE_SPEED = 2; //300;
constexpr int PADDLE_WIDTH = 2;
constexpr int PADDLE_HEIGHT = 4; //120
constexpr int BALL_RADIUS = 1;
constexpr int PADDLE_Y = (SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2);
constexpr int PADDLE1_X = 1;
constexpr int PADDLE2_X = (SCREEN_WIDTH - PADDLE_WIDTH - 1);

// Simulation constants
constexpr int RENDER_WIDTH = 10 * SCREEN_WIDTH;
constexpr int RENDER_HEIGHT = 10 * SCREEN_HEIGHT;

