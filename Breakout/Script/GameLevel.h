#pragma once

#include <string>
#include <vector>

#include <Learning2DEngine/Render/Resolution.h>

#include "BrickController.h"

class GameLevel
{
private:
    const std::string fileName;
    unsigned int levelHeight;
    unsigned int levelWidth;

    void Init(const std::vector<std::vector<unsigned int>>& brickData);
    void ClearBricks();
    void CalcBrickSize(const Learning2DEngine::Render::Resolution& resolution);
public:
    std::vector<BrickController*> bricks;

    GameLevel(const std::string& fileName);
    void Load();
    void Draw();
    bool IsCompleted();
};