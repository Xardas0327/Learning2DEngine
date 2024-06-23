#pragma once

#include <glm/glm.hpp>

#include "Learning2DEngine/System/GameObject.h"

class BrickController : public virtual Learning2DEngine::System::Component
{
public:
    glm::vec2   velocity;
    bool        isSolid;
};