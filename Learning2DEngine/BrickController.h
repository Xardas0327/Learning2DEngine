#pragma once

#include "Learning2DEngine/System/Component.h"
#include "Learning2DEngine/System/GameObject.h"

class BrickController : public virtual Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
    BrickController(Learning2DEngine::System::GameObject* gameObject, bool isSolid = false)
        : Component(gameObject), isSolid(isSolid)
    {

    }
public:
    bool isSolid;
};