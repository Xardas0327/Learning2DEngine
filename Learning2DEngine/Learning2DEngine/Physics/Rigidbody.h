#pragma once

#include <glm/glm.hpp>

#include "../System/Component.h"
#include "../System/GameObject.h"
#include "../System/Game.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        class Rigidbody : public virtual System::Component
        {
            friend class System::GameObject;
        protected:
            Rigidbody(System::GameObject* gameObject, glm::vec2 velocity = glm::vec2(0.0f, 0.0f))
                : System::Component(gameObject), velocity(velocity), isFrozen(false)
            {

            }

            Rigidbody(System::GameObject* gameObject, bool isFrozen)
                : System::Component(gameObject), velocity(glm::vec2(0.0f, 0.0f)), isFrozen(isFrozen)
            {

            }
            // TODO: This function have to be removed
            void Init() override {};
            // TODO: This function have to be removed
            void Destroy() override {};
        public:
            glm::vec2 velocity;
            bool isFrozen;

            /// <summary>
            /// If it is not frozen, the position of gameobject will be updated.
            /// </summary>
            void Update()
            {
                if (!isFrozen)
                {
                    gameObject->transform.position += velocity * System::Game::GetDeltaTime();
                }
            }
        };
    }
}