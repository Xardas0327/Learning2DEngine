#pragma once

#include <glm/glm.hpp>

#include "../System/Component.h"
#include "../System/GameObject.h"
#include "../System/Game.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        class Rigidbody : public virtual Learning2DEngine::System::Component
        {
            friend class Learning2DEngine::System::GameObject;
        public:
            glm::vec2 velocity;
            bool isFrozen;

            Rigidbody(Learning2DEngine::System::GameObject* gameObject, glm::vec2 velocity = glm::vec2(0.0f, 0.0f))
                : Component(gameObject), velocity(velocity), isFrozen(false)
            {

            }

            Rigidbody(Learning2DEngine::System::GameObject* gameObject, bool isFrozen)
                : Component(gameObject), velocity(glm::vec2(0.0f, 0.0f)), isFrozen(isFrozen)
            {

            }

            /// <summary>
            /// If it is not frozen, the position of gameobject will be updated.
            /// </summary>
            void Update()
            {
                if (!isFrozen)
                {
                    gameObject->transform.position += velocity * Learning2DEngine::System::Game::GetDeltaTime();
                }
            }
        };
    }
}