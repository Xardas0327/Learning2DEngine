#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>

class BrickController : public virtual Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
    const bool isSolid;

    BrickController(Learning2DEngine::System::GameObject* gameObject, int x, int y, bool isSolid = false);
    void Init() override;
public:
    int mapCoorX;
    int mapCoorY;
    Learning2DEngine::Physics::BoxColliderComponent* collider;
    Learning2DEngine::Render::SpriteRenderer* renderer;

    inline bool IsSolid()
    {
        return isSolid;
    }
};