#include "BallController.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/Render/RenderManager.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;

BallController::BallController(GameObject* gameObject, float radius)
    : Component(gameObject), rigidbody(nullptr), collider(nullptr), radius(radius),
        stuck(true), sticky(false), passThrough(false)
{

}

void BallController::Init()
{
    rigidbody = gameObject->AddComponent<Rigidbody, glm::vec2>(INITIAL_BALL_VELOCITY);
    collider = gameObject->AddComponent<CircleCollider, glm::vec2, float>(glm::vec2(0.0f, 0.0f), radius);
}

void BallController::Move()
{
    if (!stuck)
    {
        int windowWidth = RenderManager::GetInstance().GetResolution().GetWidth();
        gameObject->transform.position += rigidbody->velocity * Game::GetDeltaTime();

        if (gameObject->transform.position.x <= 0.0f)
        {
            rigidbody->velocity.x = -rigidbody->velocity.x;
            gameObject->transform.position.x = 0.0f;
        }
        else if (gameObject->transform.position.x + gameObject->transform.scale.x >= windowWidth)
        {
            rigidbody->velocity.x = -rigidbody->velocity.x;
            gameObject->transform.position.x = windowWidth - gameObject->transform.scale.x;
        }
        if (gameObject->transform.position.y <= 0.0f)
        {
            rigidbody->velocity.y = -rigidbody->velocity.y;
            gameObject->transform.position.y = 0.0f;
        }
    }
}

void BallController::Reset(glm::vec2 position, glm::vec2 velocity)
{
    gameObject->transform.position = position;
    rigidbody->velocity = velocity;
    stuck = true;
    sticky = false;
    passThrough = false;
}