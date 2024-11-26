#include "Ball.hpp"
#include "constants.hpp"

Ball::Ball() : x(SCREEN_WIDTH / 2), y(SCREEN_HEIGHT / 2), radius(BALL_RADIUS), speed_x(BALL_SPEED), speed_y(BALL_SPEED) {}

void Ball::Reset() {
    x = SCREEN_WIDTH / 2;
    y = SCREEN_HEIGHT / 2;
    speed_x = 0; // Stop movement initially
    speed_y = 0; // Stop movement initially
}

void Ball::StartMoving() {
    speed_x = (GetRandomValue(0, 1) == 0 ? -BALL_SPEED : BALL_SPEED);
    speed_y = (GetRandomValue(0, 1) == 0 ? -BALL_SPEED : BALL_SPEED);
}

void Ball::Update(float deltaTime) {
    x += speed_x * deltaTime;
    y += speed_y * deltaTime;

    if (y - radius <= 0 || y + radius >= SCREEN_HEIGHT) {
        speed_y *= -1;
    }
}

void Ball::Draw() const {
    DrawCircle(static_cast<int>(x), static_cast<int>(y), radius, WHITE);
}
