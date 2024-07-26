#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/BoxCollider.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>

class BrickController : public virtual Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
    BrickController(Learning2DEngine::System::GameObject* gameObject, int x, int y, bool isSolid = false);
    void Init() override;
public:
    bool isSolid;
    int x;
    int y;
    Learning2DEngine::Physics::BoxCollider* collider;
    Learning2DEngine::Render::SpriteRenderer* renderer;
};