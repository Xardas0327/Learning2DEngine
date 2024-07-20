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
            Rigidbody* rigidbody;
            bool isTriggerOnly;
        public:
            glm::vec2 center;

            Collider(Learning2DEngine::System::GameObject* gameObject, glm::vec2 center, bool isTriggerOnly = false)
                : Learning2DEngine::System::Component(gameObject), rigidbody(nullptr),
                center(center), isTriggerOnly(isTriggerOnly)
            {

            }

            void Init() override
            {
                if (!isTriggerOnly)
                {
                    rigidbody = gameObject->GetComponent<Rigidbody>();
                    if (rigidbody == nullptr)
                    {
                        LOG_ERROR("Collider: The Collider is not trigger, but the GameObject doesn't have Rigidbody.");
                    }
                }
            }

            inline bool IsTriggerOnly()
            {
                return isTriggerOnly;
            }
        };
    }
}