#pragma once

#include <glm/glm.hpp>

#include "../System/Transform.h"

namespace Learning2DEngine
{
	namespace Render
	{
        struct Particle {
            Learning2DEngine::System::Transform transform;
            glm::vec2 velocity;
            glm::vec4 color;
            float life;
            float speed;

            Particle() 
                : transform(), velocity(0.0f), color(1.0f), life(0.0f), speed(0.0f)
            {
            }
        };
	}
}