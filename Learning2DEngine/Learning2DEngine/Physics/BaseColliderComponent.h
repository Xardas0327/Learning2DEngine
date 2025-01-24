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
        class BaseColliderComponent : public virtual System::Component
        {
        private:
            const bool isPassive;

        protected:
            BaseColliderComponent(
                System::GameObject* gameObject,
                bool isPassive,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f),
                int32_t maskLayer = ~0)
                : System::Component(gameObject), isPassive(isPassive), offset(offset), maskLayer(maskLayer)
            {

            }
        public:
            glm::vec2 offset;
			int32_t maskLayer;

            glm::vec2 GetCenter() const
            {
                glm::vec2 position = gameObject->transform.position;
                position.x += gameObject->transform.scale.x / 2 + offset.x;
                position.y += gameObject->transform.scale.y / 2 + offset.y;

                return position;
            }

            inline bool IsPassive() const
            {
                return isPassive;
            }

            virtual void OnCollision(Collision collision) = 0;
        };
    }
}
