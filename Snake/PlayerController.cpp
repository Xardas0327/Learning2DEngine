#include "PlayerController.h"

#include <Learning2DEngine/System/ResourceManager.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;

PlayerController::PlayerController(GameObject* gameObject, const std::string& textureId)
	: Component(gameObject), textureId(textureId), initSize(INIT_SNAKE_LENGTH), color(PLAYER_COLOR), snake()
{

}

void PlayerController::Destroy()
{
    for (auto it = snake.begin(); it != snake.end(); ++it) {
        GameObject::Destroy(*it);
    }
    snake.clear();
}

void PlayerController::Regenerate(glm::ivec2 unitSize)
{
    for (auto it = snake.begin(); it != snake.end(); ++it) {
        GameObject::Destroy(*it);
    }
    snake.clear();
    for (unsigned int i = 0; i < initSize; ++i)
    {
        snake.push_front(CreateNewSnakeUnit(glm::vec2(i * unitSize.x, 0.0f), unitSize));
    }
}

SimpleSpriteRenderComponent* PlayerController::CreateNewSnakeUnit(glm::vec2 position, glm::vec2 scale)
{
    auto playerUnit = GameObject::Create(
        Transform(
            position,
            scale
        )
    );
    return playerUnit->AddComponent<SimpleSpriteRenderComponent, const Texture2D&, int, glm::vec4>(
        ResourceManager::GetInstance().GetTexture(textureId),
        0,
        color
    );
}

bool PlayerController::IsInSnake(glm::vec2 position)
{
    for (auto it = snake.begin(); it != snake.end(); ++it) {
        if ((*it)->gameObject->transform.GetPosition() == position)
        {
            return true;
        }
    }
    return false;
}

void PlayerController::IncreaseSize(glm::vec2 headPosition, glm::ivec2 unitSize)
{
    snake.push_front(CreateNewSnakeUnit(headPosition, unitSize));
}

void PlayerController::Move(glm::vec2 headPosition)
{
    auto end = snake.rbegin();
    auto beforeIt = snake.rbegin();
    ++beforeIt;
    while (beforeIt != snake.rend())
    {
        (*end)->gameObject->transform.SetPosition(
            (*beforeIt)->gameObject->transform.GetPosition()
        );
        ++end;
        ++beforeIt;
    }
    snake.front()->gameObject->transform.SetPosition(headPosition);
}