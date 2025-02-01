#pragma once

#include <glm/glm.hpp>

#include "../System/UpdaterComponent.h"
#include "../System/GameObject.h"
#include "../System/Game.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        class Rigidbody : public virtual System::UpdaterComponent
        {
            friend class System::GameObject;
        protected:
            Rigidbody(System::GameObject* gameObject, glm::vec2 velocity = glm::vec2(0.0f, 0.0f), bool isFrozen = false)
                : System::UpdaterComponent(gameObject), System::BaseUpdaterComponent(gameObject), System::Component(gameObject),
                velocity(velocity), isFrozen(isFrozen)
            {

            }

            Rigidbody(System::GameObject* gameObject, bool isFrozen)
                : System::UpdaterComponent(gameObject), System::BaseUpdaterComponent(gameObject), System::Component(gameObject),
                velocity(glm::vec2(0.0f, 0.0f)), isFrozen(isFrozen)
            {

            }
        public:
            glm::vec2 velocity;
            bool isFrozen;

            /// <summary>
            /// If it is not frozen, the position of gameobject will be updated.
            /// </summary>
            virtual void Update() override
            {
                if (!isFrozen)
                {
                    gameObject->transform.position += velocity * System::Game::GetDeltaTime();
                }
            }
        };
    }
}