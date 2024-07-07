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
            //In seconds. The negative number is same as 0.
            float lifeTime;
            float speed;

            Particle() 
                : transform(), velocity(0.0f), color(1.0f), lifeTime(0.0f), speed(1.0f)
            {
            }
        };
	}
}