#pragma once
#include "raylib.h"

class ScoreManager {
private:
    int leftScore;
    int rightScore;

public:
    ScoreManager();

    void LeftScored();
    void RightScored();
    void DrawScore() const;

    int GetLeftScore() const;
    int GetRIghtScore() const;
};

