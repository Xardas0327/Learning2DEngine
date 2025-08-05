#pragma once

#include <glm/glm.hpp>

#include "../System/Transform.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
#define L2DE_PARTICLE_UV_DEFAULT glm::mat2x4 { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f}

        struct Particle
        {
            System::Transform transform;
            glm::vec2 velocity;
            glm::vec4 color;
            //In seconds. The negative number is same as 0.
            float lifeTime;
            float speed;

            /// <summary>
            /// The texture coordinate order:
            /// Top Left,
            /// Top Right,
            /// Bottom Right,
            /// Bottom Left
            /// </summary>
            glm::mat2x4 uvMatrix;

            Particle() 
                : transform(), velocity(0.0f), color(1.0f), lifeTime(0.0f), speed(1.0f),
                uvMatrix(L2DE_PARTICLE_UV_DEFAULT)
            {
            }
        };
	}
}