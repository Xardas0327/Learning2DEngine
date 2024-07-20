#pragma once

#include <glm/glm.hpp>

#include "../System/Component.h"
#include "../System/GameObject.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        class Rigidbody : public virtual Learning2DEngine::System::Component
        {
            friend class Learning2DEngine::System::GameObject;
        public:
            glm::vec2 velocity;

            Rigidbody(Learning2DEngine::System::GameObject* gameObject, glm::vec2 velocity = glm::vec2(0.0f, 0.0f))
                : Component(gameObject), velocity(velocity)
            {

            }
        };
    }
}