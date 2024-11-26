#include "Ball.hpp"
#include "Paddle.hpp"
#include "raylib.h"
#include "ScoreManager.hpp"

class Game {
private:
    Ball ball;
    PlayerPaddle playerPaddle;
    CpuPaddle cpuPaddle;
    ScoreManager scoreManager;

    float resetTimer;        // Timer for the ball reset delay
    bool isBallFrozen;       // Flag to track if the ball is frozen

public:
    Game();
    ~Game();
    void Run();

private:
    void Update(float deltaTime);
    void Render() const;
};
