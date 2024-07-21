#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"
#include "Collider.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        class CircleCollider : public virtual Collider
        {
            friend class Learning2DEngine::System::GameObject;
        public:
            float radius;

            CircleCollider(Learning2DEngine::System::GameObject* gameObject, float radius, glm::vec2 offset = glm::vec2(0.0f, 0.0f))
                : Learning2DEngine::System::Component(gameObject),
                Collider(gameObject, offset), radius(radius)
            {

            }
        };
    }
}