#pragma once

#include <glm/glm.hpp>

#include "Learning2DEngine/System/Game.h"
#include "Learning2DEngine/System/Component.h"
#include "Learning2DEngine/System/GameObject.h"

class BallController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    BallController(Learning2DEngine::System::GameObject* gameObject, float radius, glm::vec2 velocity = glm::vec2(0.0f, 0.0f))
        : Component(gameObject), velocity(velocity),
        radius(radius), stuck(true), sticky(false), passThrough(false)
    {

    }
public:
    glm::vec2 velocity;
    float radius;
    bool stuck;
    bool sticky;
    bool passThrough;

    glm::vec2 Move(unsigned int window_width)
    {
        // if not stuck to player board
        if (!stuck)
        {
            // move the ball
            gameObject->transform.position += velocity * Learning2DEngine::System::Game::GetDeltaTime();
            // then check if outside window bounds and if so, reverse velocity and restore at correct position
            if (gameObject->transform.position.x <= 0.0f)
            {
                velocity.x = -velocity.x;
                gameObject->transform.position.x = 0.0f;
            }
            else if (gameObject->transform.position.x + gameObject->transform.scale.x >= window_width)
            {
                velocity.x = -velocity.x;
                gameObject->transform.position.x = window_width - gameObject->transform.scale.x;
            }
            if (gameObject->transform.position.y <= 0.0f)
            {
                velocity.y = -velocity.y;
                gameObject->transform.position.y = 0.0f;
            }
        }
        return gameObject->transform.position;
    }

    // resets the ball to initial Stuck Position (if ball is outside window bounds)
    void Reset(glm::vec2 position, glm::vec2 velocity)
    {
        gameObject->transform.position = position;
        this->velocity = velocity;
        stuck = true;
        sticky = false;
        passThrough = false;
    }
};