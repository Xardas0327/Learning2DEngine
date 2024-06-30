#include "GameLevel.h"

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

#include "Learning2DEngine/System/ResourceManager.h"
#include "Learning2DEngine/System/GameObject.h"
#include "Learning2DEngine/Render/SpriteRenderer.h"
#include "Learning2DEngine/Render/Texture2D.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

GameLevel::GameLevel()
	: bricks()
{

}

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
    ClearBricks();

    std::ifstream fstream(file);
    if (fstream)
    {
        std::string line;
        std::vector<std::vector<unsigned int>> brickData;

        while (std::getline(fstream, line))
        {
            unsigned int brickCode;
            std::vector<unsigned int> row;
            std::istringstream sstream(line);

            while (sstream >> brickCode)
            {
                row.push_back(brickCode);
            }
            brickData.push_back(row);
        }
        if (brickData.size() > 0)
            this->Init(brickData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw()
{
    for (BrickController* brick : this->bricks)
    {
        if (brick->gameObject->isActive)
            brick->gameObject->GetRenderer()->Draw();
    }
}

bool GameLevel::IsCompleted()
{
    for (BrickController* brick : this->bricks)
    {
        if (!brick->isSolid && brick->gameObject->isActive)
            return false;
    }

    return true;
}

void GameLevel::Init(const std::vector<std::vector<unsigned int>>& brickData, unsigned int levelWidth, unsigned int levelHeight)
{
    unsigned int height = brickData.size();
    unsigned int width = brickData[0].size();

    float unit_width = static_cast<float>(levelWidth) / static_cast<float>(width);
    float unit_height = static_cast<float>(levelHeight) / static_cast<float>(height);

    auto& resourceManager = ResourceManager::GetInstance();

    Texture2D solidBlockTexture = resourceManager.GetTexture("block_solid");
    Texture2D blockTexture = resourceManager.GetTexture("block");

	
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            if (brickData[y][x] == 0)
                continue;

            glm::vec2 position(unit_width * x, unit_height * y);
            glm::vec2 scale(unit_width, unit_height);

            //white
            glm::vec3 color = glm::vec3(1.0f);
            if (brickData[y][x] == 1)
                color = glm::vec3(0.8f, 0.8f, 0.7f);
            else if (brickData[y][x] == 2)
                color = glm::vec3(0.2f, 0.6f, 1.0f);
            else if (brickData[y][x] == 3)
                color = glm::vec3(0.0f, 0.7f, 0.0f);
            else if (brickData[y][x] == 4)
                color = glm::vec3(0.8f, 0.8f, 0.4f);
            else if (brickData[y][x] == 5)
                color = glm::vec3(1.0f, 0.5f, 0.0f);

            GameObject* brick = new GameObject(
                Transform(position, scale)
            );

            brick->AddRenderer<SpriteRenderer, const Texture2D&, glm::vec3>(
                brickData[y][x] == 1 // solid
                ? solidBlockTexture
                : blockTexture,
                color);

            auto brickController = brick->AddBehaviour<BrickController, bool>(brickData[y][x] == 1);
            this->bricks.push_back(brickController);
        }
    }
}

void GameLevel::ClearBricks()
{
    for (BrickController* brick : this->bricks)
    {
        GameObject::Destroy(brick);
    }
    this->bricks.clear();
}