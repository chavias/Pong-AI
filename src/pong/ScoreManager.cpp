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
    int fontSize = SCREEN_HEIGHT / 5;  
    int yOffset = SCREEN_HEIGHT / 20; 

    DrawText(TextFormat("%i", leftScore), SCREEN_WIDTH / 4 - fontSize, yOffset, fontSize, Fade(WHITE, 0.5));
    DrawText(TextFormat("%i", rightScore), 3 * SCREEN_WIDTH / 4, yOffset, fontSize, Fade(WHITE, 0.5));
}


int ScoreManager::GetLeftScore() const {
    return leftScore;
}

int ScoreManager::GetRIghtScore() const {
    return rightScore;
}


void ScoreManager::ResetScore()
{
    leftScore = 0;
    rightScore = 0;
}