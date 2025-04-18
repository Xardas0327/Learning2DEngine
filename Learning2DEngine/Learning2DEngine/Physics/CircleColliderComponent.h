#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"
#include "../System/ComponentManager.h"
#include "BaseCircleColliderComponent.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The CircleColliderComponent is really basic.
        /// It doesn't rotate, scale with the gameobject.
		/// Please check for more information in the BaseColliderComponent and BaseCircleColliderComponent.
        /// </summary>
        class CircleColliderComponent : public virtual BaseCircleColliderComponent
        {
            friend class System::GameObject;
        protected:
            CircleColliderComponent(
                System::GameObject* gameObject,
                float radius,
                ColliderType type = ColliderType::DYNAMIC,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
                : System::Component(gameObject), BaseColliderComponent(gameObject, type, offset, maskLayer),
				BaseCircleColliderComponent(gameObject, radius, type, offset, maskLayer)
            {

            }

            /// <summary>
            /// If this function is override, it must call the CircleColliderComponent::Init() in the first line.
            /// </summary>
            virtual void Init() override
            {
				System::ComponentManager::GetInstance().AddToCollider(this);
            }

            /// <summary>
            /// If this function is override, it must call the CircleColliderComponent::Destroy() in the first line.
            /// </summary>
            virtual void Destroy() override
            {
				System::ComponentManager::GetInstance().RemoveFromCollider(this);
            }
        };
    }
}
