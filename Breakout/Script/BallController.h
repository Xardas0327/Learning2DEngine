#pragma once

#include <string>

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>
#include <Learning2DEngine/Physics/Rigidbody.h>
#include <Learning2DEngine/Physics/CircleCollider.h>
#include <Learning2DEngine/ParticleSimulator/ParticleSystem.h>

#include "PlayerController.h"

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

class BallController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    const std::string textureId;
    const std::string particleTextureId;
    PlayerController* playerController;

    BallController(Learning2DEngine::System::GameObject* gameObject, PlayerController* playerController,
        const std::string& textureId, const std::string& particleTextureId);
    void Init() override;
    // TODO: This function have to be removed
    void Destroy() override {};
    void InitParticleSystem();
public:
    Learning2DEngine::Render::SpriteRenderer* renderer;
    Learning2DEngine::Physics::Rigidbody* rigidbody;
    Learning2DEngine::Physics::CircleCollider* collider;
    Learning2DEngine::ParticleSimulator::ParticleSystem* particleSystem;
    float radius;
    bool stuck;
    bool sticky;
    bool passThrough;

    void Move();
    void Reset();
};