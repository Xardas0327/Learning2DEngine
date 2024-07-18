#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>

class BallController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    BallController(Learning2DEngine::System::GameObject* gameObject, float radius, glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

public:
    glm::vec2 velocity;
    float radius;
    bool stuck;
    bool sticky;
    bool passThrough;

    void Move();
    void Reset(glm::vec2 position, glm::vec2 velocity);
};