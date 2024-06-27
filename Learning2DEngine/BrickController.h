#pragma once

#include <glm/glm.hpp>

#include "Learning2DEngine/System/Behaviour.h"

class BrickController : public virtual Learning2DEngine::System::Behaviour
{
	friend class Learning2DEngine::System::GameObject;
protected:
    BrickController(Learning2DEngine::System::GameObject* gameObject)
        : Component(gameObject), Behaviour(gameObject), isSolid(false)
    {

    }
public:
    bool isSolid;
};