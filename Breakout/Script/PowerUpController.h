#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>
#include <Learning2DEngine/Render/Texture2D.h>
#include <Learning2DEngine/Physics/Rigidbody.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/EventSystem/EventHandler.h>

#include "PowerUpType.h"
#include "PowerUpObject.h"
#include "PowerUpActivationEventItem.h"

const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUpController : public virtual Learning2DEngine::Physics::BoxColliderComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    Learning2DEngine::EventSystem::EventHandler<PowerUpType> activationEventHandler;

    PowerUpController(Learning2DEngine::System::GameObject* gameObject, const PowerUpObject& powerUpObject);

    void Init() override;
public:
    Learning2DEngine::Physics::Rigidbody* rigidbody;
    Learning2DEngine::Render::SpriteRenderer* renderer;
    const PowerUpObject powerUpObject;
    float actualDuration;
    bool activated;

    void OnCollision(Learning2DEngine::Physics::Collision collision) override;

    static PowerUpController* CreatePowerUp(
        const PowerUpObject& powerUpObject,
        glm::vec2 position,
        PowerUpActivationEventItem& eventItem);

};