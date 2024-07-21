#pragma once

#include <glm/glm.hpp>

#include "../System/Component.h"
#include "../System/GameObject.h"
#include "../DebugTool/Log.h"

#include "Rigidbody.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        class Collider : public virtual Learning2DEngine::System::Component
        {
            friend class Learning2DEngine::System::GameObject;
        protected:
            bool isTriggerOnly;
        public:
            glm::vec2 offset;
            Rigidbody* const rigidbody;

            Collider(Learning2DEngine::System::GameObject* gameObject, glm::vec2 offset = glm::vec2(0.0f, 0.0f), bool isTriggerOnly = false)
                : Learning2DEngine::System::Component(gameObject), rigidbody(gameObject->GetComponent<Rigidbody>()),
                offset(offset), isTriggerOnly(isTriggerOnly)
            {

            }

            void Init() override
            {
                if (!isTriggerOnly && rigidbody == nullptr)
                {
                    LOG_WARNING("Collider: The Collider is not trigger, but the GameObject doesn't have Rigidbody.");
                }
            }

            inline glm::vec2 GetCenter() const
            {
                return gameObject->transform.position + offset;
            }

            inline bool IsTriggerOnly() const
            {
                return isTriggerOnly;
            }

            inline bool IsFrozen() const
            {
                return rigidbody == nullptr 
                    || (rigidbody != nullptr && rigidbody->isFrozen);
            }
        };
    }
}