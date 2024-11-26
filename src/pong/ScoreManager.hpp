#pragma once
#include "raylib.h"

class ScoreManager {
private:
    int player_score;
    int cpu_score;

public:
    ScoreManager();

    void PlayerScored();
    void CpuScored();
    void DrawScore() const;

    int GetPlayerScore() const;
    int GetCpuScore() const;
};

