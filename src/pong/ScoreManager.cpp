#include "ScoreManager.hpp"
#include "constants.hpp"

ScoreManager::ScoreManager() : leftScore(0), rightScore(0) {}

void ScoreManager::LeftScored() {
    leftScore++;
}

void ScoreManager::RightScored() {
    rightScore++;
}

void ScoreManager::DrawScore() const {
    DrawText(TextFormat("%i", rightScore), SCREEN_WIDTH / 4, 20, 80, Fade(WHITE, 0.5));
    DrawText(TextFormat("%i", leftScore), 3 * SCREEN_WIDTH / 4 - 20, 20, 80, Fade(WHITE, 0.5));
}

int ScoreManager::GetLeftScore() const {
    return leftScore;
}

int ScoreManager::GetRIghtScore() const {
    return rightScore;
}
