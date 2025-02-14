#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"
#include "../System/ObjectManager.h"
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
                bool isActiveCollider = true,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
				: System::Component(gameObject), BaseColliderComponent(gameObject, isActiveCollider, offset, maskLayer),
				BaseBoxColliderComponent(gameObject, size, isActiveCollider, offset, maskLayer)
            {

            }

            /// <summary>
            /// If this function is override, it must call the BoxColliderComponent::Init() in the first line.
            /// </summary>
            virtual void Init() override
            {
                System::ObjectManager::GetInstance().AddToCollider(this);
            }

            /// <summary>
            /// If this function is override, it must call the BoxColliderComponent::Destroy() in the first line.
            /// </summary>
            virtual void Destroy() override
            {
                System::ObjectManager::GetInstance().RemoveFromCollider(this);
            }
        };
    }
}
