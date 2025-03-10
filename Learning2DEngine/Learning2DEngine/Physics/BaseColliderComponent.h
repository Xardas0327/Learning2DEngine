#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include "../System/Component.h"
#include "../System/GameObject.h"

#include "Collision.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        /// <summary>
        /// The classes, which are inherited from BaseColliderComponent,
        /// have to have a constructor, which first parameter is GameObject* for gameObject member.
        /// Moreover, it is recommended, that the constructor, the Init() and Destroy() of the inherited class is protected
        /// and only the GameObject can use them.
        /// </summary>
        class BaseColliderComponent : public virtual System::Component
        {
        private:
            const bool isKinematic;

        protected:
            BaseColliderComponent(
                System::GameObject* gameObject,
                bool isKinematic = false,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
                : System::Component(gameObject), isKinematic(isKinematic), colliderOffset(offset), maskLayer(maskLayer)
            {

            }
        public:
            glm::vec2 colliderOffset;
            int32_t maskLayer;

            glm::vec2 GetColliderCenter() const
            {
                glm::vec2 position = gameObject->transform.position;
                position.x += gameObject->transform.scale.x / 2 + colliderOffset.x;
                position.y += gameObject->transform.scale.y / 2 + colliderOffset.y;

                return position;
            }

            inline bool IsKinematic() const
            {
                return isKinematic;
            }

            virtual void OnCollision(Collision collision)
            {

            }
        };
    }
}
