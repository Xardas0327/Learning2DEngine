#include "BallController.h"

#include "Learning2DEngine/System/Game.h"
#include "Learning2DEngine/Render/RenderManager.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

BallController::BallController(GameObject* gameObject, float radius, glm::vec2 velocity)
    : Component(gameObject), velocity(velocity),
    radius(radius), stuck(true), sticky(false), passThrough(false)
{

}

void BallController::Move()
{
    if (!stuck)
    {
        int windowWidth = RenderManager::GetInstance().GetResolution().GetWidth();
        gameObject->transform.position += velocity * Game::GetDeltaTime();

        if (gameObject->transform.position.x <= 0.0f)
        {
            velocity.x = -velocity.x;
            gameObject->transform.position.x = 0.0f;
        }
        else if (gameObject->transform.position.x + gameObject->transform.scale.x >= windowWidth)
        {
            velocity.x = -velocity.x;
            gameObject->transform.position.x = windowWidth - gameObject->transform.scale.x;
        }
        if (gameObject->transform.position.y <= 0.0f)
        {
            velocity.y = -velocity.y;
            gameObject->transform.position.y = 0.0f;
        }
    }
}

void BallController::Reset(glm::vec2 position, glm::vec2 velocity)
{
    gameObject->transform.position = position;
    this->velocity = velocity;
    stuck = true;
    sticky = false;
    passThrough = false;
}