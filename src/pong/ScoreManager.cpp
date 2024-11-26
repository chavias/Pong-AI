#include "ScoreManager.hpp"
#include "constants.hpp"

ScoreManager::ScoreManager() : player_score(0), cpu_score(0) {}

void ScoreManager::PlayerScored() {
    player_score++;
}

void ScoreManager::CpuScored() {
    cpu_score++;
}

void ScoreManager::DrawScore() const {
    DrawText(TextFormat("%i", cpu_score), SCREEN_WIDTH / 4, 20, 80, Fade(WHITE, 0.5));
    DrawText(TextFormat("%i", player_score), 3 * SCREEN_WIDTH / 4 - 20, 20, 80, Fade(WHITE, 0.5));
}

int ScoreManager::GetPlayerScore() const {
    return player_score;
}

int ScoreManager::GetCpuScore() const {
    return cpu_score;
}
