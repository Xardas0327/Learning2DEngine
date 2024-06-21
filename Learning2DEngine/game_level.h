#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Learning2DEngine/System/ResourceManager.h"
#include "game_object.h"


/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
    // level state
    std::vector<GameObject> Bricks;
    // constructor
    GameLevel() { }
    // loads level from file
    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight, const glm::mat4& projection);
    // render level
    void Draw();
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
private:
    // initialize level from tile data
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight, const glm::mat4& projection);
};