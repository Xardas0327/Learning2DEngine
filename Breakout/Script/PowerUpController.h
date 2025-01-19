#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>
#include <Learning2DEngine/Render/Texture2D.h>
#include <Learning2DEngine/Physics/Rigidbody.h>
#include <Learning2DEngine/Physics/BoxCollider.h>

#include "PowerUpType.h"
#include "PowerUpObject.h"

const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUpController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    PowerUpController(Learning2DEngine::System::GameObject* gameObject, const PowerUpObject& powerUpObject)
        : Learning2DEngine::System::Component(gameObject), rigidbody(nullptr), collider(nullptr), renderer(nullptr),
        powerUpObject(powerUpObject), actualDuration(powerUpObject.duration), activated(false)
    {
        gameObject->transform.scale = POWERUP_SIZE;
    }

    void Init() override
    {
        using namespace Learning2DEngine::System;
        using namespace Learning2DEngine::Render;
        using namespace Learning2DEngine::Physics;

        rigidbody = gameObject->AddComponent<Rigidbody, glm::vec2>(VELOCITY);
        collider = gameObject->AddComponent<BoxCollider, glm::vec2>(gameObject->transform.scale);
        renderer = gameObject->AddComponent<SpriteRenderer, const Texture2D&, int, glm::vec3>(
            ResourceManager::GetInstance().GetTexture(powerUpObject.textureId),
            1,
            powerUpObject.color
        );

    }
    // TODO: This function have to be removed
    void Destroy() override {};
public:
    Learning2DEngine::Physics::Rigidbody* rigidbody;
    Learning2DEngine::Physics::BoxCollider* collider;
    Learning2DEngine::Render::SpriteRenderer* renderer;
    const PowerUpObject powerUpObject;
    float actualDuration;
    bool activated;

    static PowerUpController* CreatePowerUp(const PowerUpObject& powerUpObject, glm::vec2 position)
    {
        auto powerUp = new Learning2DEngine::System::GameObject(
            Learning2DEngine::System::Transform(position)
        );

        return powerUp->AddComponent<PowerUpController, const PowerUpObject&>(powerUpObject);
    }

};