#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"
#include "BaseColliderComponent.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The BaseCircleColliderComponent is really basic.
        /// It doesn't rotate, scale with the gameobject.
		/// Please use CircleColliderComponent instead of this.
        /// </summary>
        class BaseCircleColliderComponent : public virtual BaseColliderComponent
        {
        protected:
            BaseCircleColliderComponent(
                System::GameObject* gameObject,
                float radius,
                ColliderType type = ColliderType::DYNAMIC,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
                : System::Component(gameObject), BaseColliderComponent(gameObject, type, offset, maskLayer),
                colliderRadius(radius)
            {

            }
        public:
            float colliderRadius;
        };
    }
}
