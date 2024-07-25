#pragma once

#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Physics/BoxCollider.h>

class BrickController : public virtual Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
    BrickController(Learning2DEngine::System::GameObject* gameObject, bool isSolid = false)
        : Component(gameObject), isSolid(isSolid), collider(nullptr)
    {

    }

    void Init() override
    {
        collider = gameObject->AddComponent<Learning2DEngine::Physics::BoxCollider, glm::vec2>(gameObject->transform.scale);
    }
public:
    bool isSolid;
    Learning2DEngine::Physics::BoxCollider* collider;
};