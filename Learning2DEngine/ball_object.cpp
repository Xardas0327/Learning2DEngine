#include "ball_object.h"

using namespace Learning2DEngine::Render;

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, const Learning2DEngine::Render::OldSpriteRenderer& spriteRenderer)
    : OldGameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), spriteRenderer, velocity), Radius(radius), Stuck(true), Sticky(false), PassThrough(false)
{ 
}

glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{
    // if not stuck to player board
    if (!this->Stuck)
    {
        // move the ball
        this->transform.position += this->Velocity * dt;
        // then check if outside window bounds and if so, reverse velocity and restore at correct position
        if (this->transform.position.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x;
            this->transform.position.x = 0.0f;
        }
        else if (this->transform.position.x + this->transform.scale.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x;
            this->transform.position.x = window_width - this->transform.scale.x;
        }
        if (this->transform.position.y <= 0.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->transform.position.y = 0.0f;
        }
    }
    return this->transform.position;
}

// resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->transform.position = position;
    this->Velocity = velocity;
    this->Stuck = true;
    this->Sticky = false;
    this->PassThrough = false;
}