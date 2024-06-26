#pragma once

#include <glm/glm.hpp>

#include "Learning2DEngine/System/GameObject.h"

class BallController : public virtual Learning2DEngine::System::Behaviour
{
public:
    glm::vec2 velocity;
    float radius;
    bool stuck;
    bool sticky;
    bool passThrough;

    glm::vec2 Move(float dt, unsigned int window_width)
    {
        // if not stuck to player board
        if (!stuck)
        {
            // move the ball
            gameObject->transform.position += velocity * dt;
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