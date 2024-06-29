#include "game_level.h"

#include <fstream>
#include <sstream>

#include "Learning2DEngine/Render/SpriteRenderer.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;


void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    for (BrickController* tile : this->Bricks)
    {
        GameObject::Destroy(tile);
    }
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw()
{
    for (BrickController* tile : this->Bricks)
        if (tile->gameObject->isActive)
            tile->gameObject->GetRenderer()->Draw();
}

bool GameLevel::IsCompleted()
{
    for (BrickController* tile : this->Bricks)
        if (!tile->isSolid && tile->gameObject->isActive)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height;

    auto& resourceManager = ResourceManager::GetInstance();

    // initialize level tiles based on tileData		
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            if (tileData[y][x] == 0)
                continue;

            glm::vec2 pos(unit_width * x, unit_height * y);
            glm::vec2 size(unit_width, unit_height);

            glm::vec3 color = glm::vec3(1.0f); // original: white
            if (tileData[y][x] == 1)
                color = glm::vec3(0.8f, 0.8f, 0.7f);
            else if (tileData[y][x] == 2)
                color = glm::vec3(0.2f, 0.6f, 1.0f);
            else if (tileData[y][x] == 3)
                color = glm::vec3(0.0f, 0.7f, 0.0f);
            else if (tileData[y][x] == 4)
                color = glm::vec3(0.8f, 0.8f, 0.4f);
            else if (tileData[y][x] == 5)
                color = glm::vec3(1.0f, 0.5f, 0.0f);

            GameObject* brick = new GameObject(
                Transform(
                    pos,
                    size
                )
            );

            auto texture = tileData[y][x] == 1 // solid
                ? new Texture2D(resourceManager.GetTexture("block_solid"))
                : new Texture2D(resourceManager.GetTexture("block"));

            brick->AddRenderer<SpriteRenderer, Texture2D*, glm::vec3>(texture, color);

            auto brickController = brick->AddBehaviour<BrickController, bool>(tileData[y][x] == 1);
            this->Bricks.push_back(brickController);
        }
    }
}