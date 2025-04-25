#include "Rigidbody.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        Rigidbody::Rigidbody(System::GameObject* gameObject, glm::vec2 velocity, bool isFrozen)
            : System::UpdaterComponent(gameObject), System::BaseUpdaterComponent(gameObject), System::Component(gameObject),
            velocity(velocity), isFrozen(isFrozen)
        {

        }

        Rigidbody::Rigidbody(System::GameObject* gameObject, bool isFrozen)
            : System::UpdaterComponent(gameObject), System::BaseUpdaterComponent(gameObject), System::Component(gameObject),
            velocity(glm::vec2(0.0f, 0.0f)), isFrozen(isFrozen)
        {

        }

        void Rigidbody::Update()
        {
            if (!isFrozen)
            {
                gameObject->transform.AddPosition(
                    velocity * System::Game::GetDeltaTime()
                );
            }
        }

        void Rigidbody::ResetVelocityX()
        {
            velocity.x = 0.0f;
        }

        void Rigidbody::ResetVelocityY()
        {
            velocity.y = 0.0f;
        }
    }
}