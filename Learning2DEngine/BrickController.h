#pragma once

#include <glm/glm.hpp>

#include "Learning2DEngine/System/Behaviour.h"

class BrickController : public virtual Learning2DEngine::System::Behaviour
{
public:
    bool isSolid;
};