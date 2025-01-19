#include "GameLevel.h"

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>
#include <Learning2DEngine/Render/Texture2D.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

GameLevel::GameLevel(const std::string& fileName)
	: fileName(fileName), levelHeight(0), levelWidth(0), bricks()
{

}

void GameLevel::Load(bool areBricksActive)
{
    ClearBricks();

    std::ifstream fstream(fileName);
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
            Init(brickData, areBricksActive);
    }
}

bool GameLevel::IsCompleted()
{
    for (BrickController* brick : bricks)
    {
        if (!brick->isSolid && brick->gameObject->isActive)
            return false;
    }

    return true;
}

void GameLevel::Init(const std::vector<std::vector<unsigned int>>& brickData, bool areBricksActive)
{
    levelHeight = brickData.size();
    levelWidth = brickData[0].size();
    auto& resourceManager = ResourceManager::GetInstance();

    Texture2D solidBlockTexture = resourceManager.GetTexture("block_solid");
    Texture2D blockTexture = resourceManager.GetTexture("block");

    for (unsigned int y = 0; y < levelHeight; ++y)
    {
        for (unsigned int x = 0; x < levelWidth; ++x)
        {
            if (brickData[y][x] == 0)
                continue;

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

            GameObject* brick = new GameObject(areBricksActive);
            auto brickController = brick->AddComponent<BrickController, int, int, bool>(x, y, brickData[y][x] == 1);
            brickController->renderer->texture =
                brickData[y][x] == 1
                ? new Texture2D(solidBlockTexture)
                : new Texture2D(blockTexture);
            brickController->renderer->color = color;

            bricks.push_back(brickController);
        }
    }

    CalcBrickSize(Game::mainCamera.GetResolution());
}

void GameLevel::CalcBrickSize(const Resolution& resolution)
{
    float unit_width = static_cast<float>(resolution.GetWidth()) / static_cast<float>(levelWidth);
    float unit_height = static_cast<float>(resolution.GetHeight() / 2) / static_cast<float>(levelHeight);
    glm::vec2 scale(unit_width, unit_height);

    for (BrickController* brick : bricks)
    {
        brick->gameObject->transform.position = glm::vec2(unit_width * brick->x, unit_height * brick->y);
        brick->gameObject->transform.scale = scale;
        brick->collider->size = scale;
    }
}

void GameLevel::ClearBricks()
{
    for (BrickController* brick : bricks)
    {
        GameObject::Destroy(brick);
    }
    bricks.clear();
}

void GameLevel::SetBricksActive(bool isActive)
{
    for (BrickController* brick : bricks)
    {
        brick->gameObject->isActive = isActive;
    }
}