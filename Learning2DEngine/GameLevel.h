#pragma once

#include <vector>

#include "BrickController.h"

class GameLevel
{
private:
    void Init(const std::vector<std::vector<unsigned int>>& brickData, unsigned int levelWidth, unsigned int levelHeight);
    void ClearBricks();

public:
    std::vector<BrickController*> bricks;

    GameLevel();
    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
    void Draw();
    bool IsCompleted();
};