#pragma once

#include <glm/glm.hpp>

#include "../System/Component.h"
#include "../System/GameObject.h"

namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		class ParticleComponent final : public virtual System::Component
		{
			friend class System::GameObject;
		private:
			static constexpr const glm::mat4x2 DefaultUV = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

            ParticleComponent(System::GameObject* gameObject)
                : Component(gameObject), velocity(0.0f), color(1.0f), lifeTime(0.0f), speed(1.0f),
                uvMatrix(ParticleComponent::DefaultUV)
            {
            }
        public:
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
            glm::mat4x2 uvMatrix;
		};
	}
}