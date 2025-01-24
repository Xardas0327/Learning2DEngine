#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"
#include "BaseColliderComponent.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The CircleColliderComponent is really basic.
        /// It doesn't rotate, scale with the gameobject.
        /// </summary>
        class CircleColliderComponent : public virtual BaseColliderComponent
        {
            friend class System::GameObject;
        protected:
            CircleColliderComponent(
                System::GameObject* gameObject,
                bool isPassive,
                float radius,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
                : System::Component(gameObject), BaseColliderComponent(gameObject, isPassive, offset, maskLayer), radius(radius)
            {

            }

            /// <summary>
            /// If this function is override, it must call the CircleColliderComponent::Init() in the first line.
            /// </summary>
            virtual void Init() override
            {
                //System::ComponentManager::GetInstance().Add(this);
            }

            /// <summary>
            /// If this function is override, it must call the CircleColliderComponent::Destroy() in the first line.
            /// </summary>
            virtual void Destroy() override
            {
                //System::ComponentManager::GetInstance().Remove(this);
            }
        public:
            float radius;
        };
    }
}
