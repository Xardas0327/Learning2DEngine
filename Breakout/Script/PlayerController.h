#pragma once

#include <string>

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

class PlayerController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
    const std::string textureId;

    PlayerController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId);

    void Init() override;
public:
    Learning2DEngine::Render::SpriteRenderer* renderer;
    Learning2DEngine::Physics::BoxColliderComponent* collider;

    void Reset();
};