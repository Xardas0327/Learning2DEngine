#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>
#include <Learning2DEngine/Render/Texture2D.h>

#include "PowerUpType.h"
#include "PowerUpObject.h"

const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUpController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    PowerUpController(Learning2DEngine::System::GameObject* gameObject, PowerUpType type, float duration)
        : Learning2DEngine::System::Component(gameObject), velocity(VELOCITY),
        type(type), duration(duration), activated(false)
    {
        gameObject->transform.scale = POWERUP_SIZE;
    }
public:
    glm::vec2 velocity;
    PowerUpType type;
    float duration;
    bool activated;

    static PowerUpController* CreatePowerUp(const PowerUpObject& powerUpObject, glm::vec2 position)
    {
        using namespace Learning2DEngine::System;
        using namespace Learning2DEngine::Render;

        GameObject* powerUp = new GameObject(
            Transform(position)
        );

        powerUp->AddComponent<SpriteRenderer, const Texture2D&, glm::vec3>(
            ResourceManager::GetInstance().GetTexture(powerUpObject.textureId),
            powerUpObject.color);
        return powerUp->AddComponent<PowerUpController, PowerUpType, float>(powerUpObject.type, powerUpObject.duration);
    }

};