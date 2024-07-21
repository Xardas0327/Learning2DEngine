#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"
#include "Collider.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        class BoxCollider : public virtual Collider
        {
            friend class Learning2DEngine::System::GameObject;
        public:
            glm::vec2 size;

            BoxCollider(Learning2DEngine::System::GameObject* gameObject, glm::vec2 offset, glm::vec2 size)
                : Learning2DEngine::System::Component(gameObject), 
                Collider(gameObject, offset), size(size)
            {

            }
        };
    }
}