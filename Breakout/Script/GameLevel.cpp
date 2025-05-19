#include "GameLevel.h"

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>

#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/Game.h>
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
        std::vector<std::vector<size_t>> brickData;

        while (std::getline(fstream, line))
        {
            size_t brickCode;
            std::vector<size_t> row;
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
        if (!brick->IsSolid() && brick->gameObject->isActive)
            return false;
    }

    return true;
}

void GameLevel::Init(const std::vector<std::vector<size_t>>& brickData, bool areBricksActive)
{
    levelHeight = brickData.size();
    levelWidth = brickData[0].size();
    auto& resourceManager = ResourceManager::GetInstance();

    Texture2D solidBlockTexture = resourceManager.GetTexture("block_solid");
    Texture2D blockTexture = resourceManager.GetTexture("block");

    for (size_t y = 0; y < levelHeight; ++y)
    {
        for (size_t x = 0; x < levelWidth; ++x)
        {
            if (brickData[y][x] == 0)
                continue;

            //white
            auto color = glm::vec4(1.0f);
            if (brickData[y][x] == 1)
                color = glm::vec4(0.8f, 0.8f, 0.7f, 1.0f);
            else if (brickData[y][x] == 2)
                color = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
            else if (brickData[y][x] == 3)
                color = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
            else if (brickData[y][x] == 4)
                color = glm::vec4(0.8f, 0.8f, 0.4f, 1.0f);
            else if (brickData[y][x] == 5)
                color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

            GameObject* brick = GameObject::Create(areBricksActive);
            auto brickController = brick->AddComponent<BrickController>(x, y, brickData[y][x] == 1);
            brickController->renderer->data.SetTexture(
                brickData[y][x] == 1
                ? solidBlockTexture
                : blockTexture);
            brickController->renderer->data.color = color;

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
        brick->gameObject->transform.SetPosition(glm::vec2(unit_width * brick->mapCoorX, unit_height * brick->mapCoorY));
        brick->gameObject->transform.SetScale(scale);
        brick->collider->colliderSize = scale;
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