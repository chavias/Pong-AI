#include "Ball.hpp"
#include "constants.hpp"
#include <cmath>

Ball::Ball() : x(SCREEN_WIDTH / 2), y(SCREEN_HEIGHT / 2), radius(BALL_RADIUS), speed_x(BALL_SPEED), speed_y(BALL_SPEED) {}

void Ball::Reflect()
{
    speed_x = -speed_x;
}

void Ball::Reset() {
    x = SCREEN_WIDTH / 2;
    y = SCREEN_HEIGHT / 2;
    // speed_x = (GetRandomValue(0, 1) == 0 ? -BALL_SPEED : BALL_SPEED);
    // speed_y = (GetRandomValue(0, 1) == 0 ? -BALL_SPEED : BALL_SPEED);

    // Generate a random angle between -75 and 75 degrees or between 105 and 255 degrees 
    // to ensure the ball does not move straight up or down.
    float randomAngle;
    do {
        randomAngle = GetRandomValue(-65, 65) * (PI / 180.0f); // Convert to radians
    } while (std::abs(randomAngle) < 1e-6);

    if (GetRandomValue(0, 1) == 0) {
        randomAngle = PI - randomAngle; // Reflect the angle to the left half of the circle
    }

    // Calculate speed components based on the random angle
    int ball_speed = (int) GetRandomValue(BALL_SPEED-2,ball_speed+2);
    speed_x = BALL_SPEED * cos(randomAngle);
    speed_y = BALL_SPEED * sin(randomAngle);

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

