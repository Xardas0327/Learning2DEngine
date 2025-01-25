#pragma once

#include <glm/glm.hpp>

#include "../System/Component.h"
#include "../System/GameObject.h"
#include "../DebugTool/Log.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        // TODO: This class have to be removed
        class Collider : public virtual System::Component
        {
        protected:
            Collider(System::GameObject* gameObject, glm::vec2 offset = glm::vec2(0.0f, 0.0f))
                : System::Component(gameObject), offset(offset)
            {

            }
            // TODO: This function have to be removed
            void Init() override {};
            // TODO: This function have to be removed
            void Destroy() override {};
        public:
            glm::vec2 offset;

            glm::vec2 GetCenter() const
            {
                glm::vec2 position = gameObject->transform.position;
                position.x+= gameObject->transform.scale.x / 2 + offset.x;
                position.y+= gameObject->transform.scale.y / 2 + offset.y;

                return position;
            }
        };
    }
}