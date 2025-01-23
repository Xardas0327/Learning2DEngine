#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"
#include "Collider.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The BoxCollider is really basic.
        /// It doesn't rotate, scale with the gameobject.
        /// </summary>
        class BoxCollider : public virtual Collider
        {
            friend class System::GameObject;
        protected:
            BoxCollider(System::GameObject* gameObject, glm::vec2 size, glm::vec2 offset = glm::vec2(0.0f, 0.0f))
                : System::Component(gameObject),
                Collider(gameObject, offset), size(size)
            {

            }
        public:
            glm::vec2 size;
        };
    }
}