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
    for (std::list<SpriteRenderer*>::iterator it = snake.begin(); it != snake.end(); ++it) {
        GameObject::Destroy(*it);
    }
    snake.clear();
}

void PlayerController::Regenerate(glm::ivec2 unitSize)
{
    for (std::list<SpriteRenderer*>::iterator it = snake.begin(); it != snake.end(); ++it) {
        GameObject::Destroy(*it);
    }
    snake.clear();
    for (unsigned int i = 0; i < initSize; ++i)
    {
        snake.push_front(CreateNewSnakeUnit(glm::vec2(i * unitSize.x, 0.0f), unitSize));
    }
}

SpriteRenderer* PlayerController::CreateNewSnakeUnit(glm::vec2 position, glm::vec2 scale)
{
    auto playerUnit = new GameObject(
        Transform(
            position,
            scale
        )
    );
    return playerUnit->AddComponent<SpriteRenderer, const Texture2D&, int, glm::vec3>(
        ResourceManager::GetInstance().GetTexture(textureId),
        0,
        color
    );
}

bool PlayerController::IsInSnake(glm::vec2 position)
{
    for (std::list<SpriteRenderer*>::iterator it = snake.begin(); it != snake.end(); ++it) {
        if ((*it)->gameObject->transform.position == position)
        {
            return true;
        }
    }
    return false;
}

void PlayerController::IncreaseSize(glm::vec2 headPostion, glm::ivec2 unitSize)
{
    snake.push_front(CreateNewSnakeUnit(headPostion, unitSize));
}

void PlayerController::Move(glm::vec2 headPostion)
{
    auto end = snake.rbegin();
    auto beforeIt = snake.rbegin();
    ++beforeIt;
    while (beforeIt != snake.rend())
    {
        (*end)->gameObject->transform.position = (*beforeIt)->gameObject->transform.position;
        ++end;
        ++beforeIt;
    }
    snake.front()->gameObject->transform.position = headPostion;
}