#pragma once

#include <string>

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>
#include <Learning2DEngine/Physics/Rigidbody.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>
#include <Learning2DEngine/ParticleSimulator/ParticleSystem.h>
#include <Learning2DEngine/EventSystem/EventHandler.h>

#include "PlayerController.h"
#include "BrickController.h"
#include "BallHitPlayerEventItem.h"
#include "BallHitBrickEventItem.h"

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class BallController : public virtual Learning2DEngine::Physics::CircleColliderComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    const std::string textureId;
    const std::string particleTextureId;
    PlayerController* playerController;
    Learning2DEngine::EventSystem::EventHandler<> hitPlayerEventHandler;
    Learning2DEngine::EventSystem::EventHandler<BrickController*> hitBrickEventHandler;

    BallController(Learning2DEngine::System::GameObject* gameObject, PlayerController* playerController,
        const std::string& textureId, const std::string& particleTextureId,
        BallHitPlayerEventItem& ballHitPlayerEventItem, BallHitBrickEventItem& ballHitBrickEventItem);
    void Init() override;
    void InitParticleSystem();
    Direction VectorDirection(glm::vec2 target);
public:
    Learning2DEngine::Render::SpriteRenderer* renderer;
    Learning2DEngine::Physics::Rigidbody* rigidbody;
    Learning2DEngine::ParticleSimulator::ParticleSystem* particleSystem;
    float radius;
    bool stuck;
    bool sticky;
    bool passThrough;

    void OnCollision(Learning2DEngine::Physics::Collision collision) override;

    void Move();
    void Reset();
};