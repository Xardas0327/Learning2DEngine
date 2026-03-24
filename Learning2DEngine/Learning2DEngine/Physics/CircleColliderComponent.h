#pragma once

#include <glm/glm.hpp>

#include "../DebugTool/DebugMacro.h"
#include "../System/GameObject.h"
#include "BaseColliderComponent.h"

namespace Learning2DEngine
{
    namespace DebugTool
    {
        class DebugCircleColliderRenderComponent;
    }

    namespace Physics
    {
        /// <summary>
        /// The CircleColliderComponent is really basic.
        /// It doesn't rotate, scale with the gameobject.
		/// Please check for more information in the BaseColliderComponent.
        /// </summary>
        class CircleColliderComponent : public BaseColliderComponent
        {
            friend class System::GameObject;
        protected:
            CircleColliderComponent(
                System::GameObject* gameObject,
                float radius,
                ColliderType type = ColliderType::DYNAMIC,
                ColliderMode mode = ColliderMode::TRIGGER,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0);

            /// <summary>
            /// If this function is override, it must call the CircleColliderComponent::Init() in the first line.
            /// </summary>
            virtual void Init() override;

            /// <summary>
            /// If this function is override, it must call the CircleColliderComponent::Destroy() in the first line.
            /// </summary>
            virtual void Destroy() override;

        public:
#if L2DE_DEBUG_SHOW_COLLIDER
            DebugTool::DebugCircleColliderRenderComponent* debugTool;
#endif

            float colliderRadius;

            glm::vec2 GetColliderCenter() const override
            {
                glm::vec2 position = gameObject->transform.GetPosition() + colliderOffset;
                position.x += colliderRadius;
                position.y += colliderRadius;

                return position;
            }
        };
    }
}
