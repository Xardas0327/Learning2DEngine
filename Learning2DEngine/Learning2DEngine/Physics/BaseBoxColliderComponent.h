#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"
#include "BaseColliderComponent.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The BaseBoxColliderComponent is really basic.
        /// It doesn't rotate, scale with the gameobject.
		/// Please use BoxColliderComponent instead of this.
        /// </summary>
        class BaseBoxColliderComponent : public virtual BaseColliderComponent
        {
        protected:
            BaseBoxColliderComponent(
                System::GameObject* gameObject,
                glm::vec2 size,
                bool isKinematic = false,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
                : System::Component(gameObject), BaseColliderComponent(gameObject, isKinematic, offset, maskLayer),
                colliderSize(size)
            {

            }
        public:
            glm::vec2 colliderSize;
        };
    }
}
