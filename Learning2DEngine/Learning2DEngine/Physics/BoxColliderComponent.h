#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"
#include "../System/ComponentManager.h"
#include "BaseBoxColliderComponent.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The BoxColliderComponent is really basic.
        /// It doesn't rotate, scale with the gameobject.
		/// Please check for more information in the BaseColliderComponent and BaseBoxColliderComponent.
        /// </summary>
        class BoxColliderComponent : public virtual BaseBoxColliderComponent
        {
            friend class System::GameObject;
        protected:
            BoxColliderComponent(
                System::GameObject* gameObject,
                glm::vec2 size,
                ColliderType type = ColliderType::DYNAMIC,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
				: System::Component(gameObject), BaseColliderComponent(gameObject, type, offset, maskLayer),
				BaseBoxColliderComponent(gameObject, size, type, offset, maskLayer)
            {

            }

            /// <summary>
            /// If this function is override, it must call the BoxColliderComponent::Init() in the first line.
            /// </summary>
            virtual void Init() override
            {
                System::ComponentManager::GetInstance().AddToCollider(this);
            }

            /// <summary>
            /// If this function is override, it must call the BoxColliderComponent::Destroy() in the first line.
            /// </summary>
            virtual void Destroy() override
            {
                System::ComponentManager::GetInstance().RemoveFromCollider(this);
            }
        };
    }
}
