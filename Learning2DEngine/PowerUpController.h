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
        Learning2DEngine::System::GameObject* powerUp = new Learning2DEngine::System::GameObject(
            Learning2DEngine::System::Transform(position)
        );
        auto spriteRenderer = powerUp->AddRenderer<Learning2DEngine::Render::SpriteRenderer, glm::vec3>(color);
        spriteRenderer->texture = new Learning2DEngine::Render::Texture2D(
            Learning2DEngine::System::ResourceManager::GetInstance().GetTexture(textureId)
        );

        return powerUp->AddBehaviour<PowerUpController, std::string, float>(type, duration);
    }
};