#include "Rigidbody.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        glm::vec2 Rigidbody::gravity = L2DE_DEFAULT_GRAVITY;

        Rigidbody::Rigidbody(System::GameObject* gameObject, glm::vec2 velocity, bool isGravityEnabled, bool isFrozen)
            : System::UpdaterComponent(gameObject), System::BaseUpdaterComponent(gameObject), System::Component(gameObject),
            velocity(velocity), isFrozen(isFrozen), isGravityEnabled(isGravityEnabled), gravityMultiplier(1.0f)
        {

        }

        Rigidbody::Rigidbody(System::GameObject* gameObject, bool isFrozen)
            : System::UpdaterComponent(gameObject), System::BaseUpdaterComponent(gameObject), System::Component(gameObject),
            velocity(glm::vec2(0.0f, 0.0f)), isFrozen(isFrozen), isGravityEnabled(false), gravityMultiplier(1.0f)
        {

        }

        void Rigidbody::Update()
        {
            if (!isFrozen)
            {
                if (isGravityEnabled)
                {
                    velocity += gravity * gravityMultiplier;
                }

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