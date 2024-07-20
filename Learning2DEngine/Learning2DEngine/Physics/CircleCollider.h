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

            CircleCollider(Learning2DEngine::System::GameObject* gameObject, glm::vec2 center, float radius, bool isTriggerOnly = false)
                : Learning2DEngine::System::Component(gameObject),
                Collider(gameObject, center, isTriggerOnly), radius(radius)
            {

            }
        };
    }
}