#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/Rigidbody.h>

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

class BallController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    BallController(Learning2DEngine::System::GameObject* gameObject, float radius);

public:
    Learning2DEngine::Physics::Rigidbody* rigidbody;
    float radius;
    bool stuck;
    bool sticky;
    bool passThrough;

    void Init() override;
    void Move();
    void Reset(glm::vec2 position, glm::vec2 velocity);
};