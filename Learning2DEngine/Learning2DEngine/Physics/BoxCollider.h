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
        /// It doesn't rotate with the gameobject and the size doesn't scale with the gameobject too.
        /// </summary>
        class BoxCollider : public virtual Collider
        {
            friend class Learning2DEngine::System::GameObject;
        public:
            glm::vec2 size;

            BoxCollider(Learning2DEngine::System::GameObject* gameObject, glm::vec2 size, glm::vec2 offset = glm::vec2(0.0f, 0.0f))
                : Learning2DEngine::System::Component(gameObject), 
                Collider(gameObject, offset), size(size)
            {

            }
        };
    }
}