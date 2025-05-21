#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

class BrickController : public Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
    const bool isSolid;

    BrickController(Learning2DEngine::System::GameObject* gameObject, size_t x, size_t y, bool isSolid = false);
    void Init() override;
public:
    size_t mapCoorX;
    size_t mapCoorY;
    Learning2DEngine::Physics::BoxColliderComponent* collider;
    Learning2DEngine::Render::SpriteRenderComponent* renderer;

    inline bool IsSolid()
    {
        return isSolid;
    }
};