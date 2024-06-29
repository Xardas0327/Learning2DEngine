#pragma once
#pragma once

#include <string>

#include <glm/glm.hpp>

#include "Learning2DEngine/System/Behaviour.h"
#include "Learning2DEngine/System/GameObject.h"
#include "Learning2DEngine/Render/SpriteRenderer.h"
#include "Learning2DEngine/Render/Texture2D.h"

const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUpController : public virtual Learning2DEngine::System::Behaviour
{
    friend class Learning2DEngine::System::GameObject;
protected:
    PowerUpController(Learning2DEngine::System::GameObject* gameObject, std::string type, float duration)
        : Learning2DEngine::System::Component(gameObject), Learning2DEngine::System::Behaviour(gameObject), velocity(VELOCITY),
        type(type), duration(duration), activated(false)
    {
        gameObject->transform.scale = POWERUP_SIZE;
    }
public:
    glm::vec2 velocity;
    std::string type;
    float duration;
    bool activated;

    static PowerUpController* CreatePowerUp(const std::string& type, float duration, glm::vec2 position, const std::string& textureId, glm::vec3 color)
    {
        using namespace Learning2DEngine::System;
        using namespace Learning2DEngine::Render;

        GameObject* powerUp = new GameObject(
            Transform(position)
        );
        auto texture = new Texture2D(
            ResourceManager::GetInstance().GetTexture(textureId)
        );
        powerUp->AddRenderer<SpriteRenderer, Texture2D*, glm::vec3>(texture, color);
        return powerUp->AddBehaviour<PowerUpController, std::string, float>(type, duration);
    }

};