#pragma once

#include <glm/glm.hpp>

#include "../System/UpdaterComponent.h"
#include "../System/GameObject.h"
#include "../System/Game.h"

#include "BaseRigidbody.h"

namespace Learning2DEngine
{
    namespace Physics
    {
#define L2DE_DEFAULT_GRAVITY glm::vec2(0.0f, 9.81f)

        class Rigidbody : public virtual System::UpdaterComponent, public BaseRigidbody
        {
            friend class System::GameObject;
        protected:
            Rigidbody(System::GameObject* gameObject, glm::vec2 velocity = glm::vec2(0.0f, 0.0f), bool isGravityEnabled = false, bool isFrozen = false);

            Rigidbody(System::GameObject* gameObject, bool isFrozen);

            /// <summary>
            /// If it is not frozen, the position of gameobject will be updated.
            /// </summary>
            virtual void Update() override;
        public:
            bool isFrozen;
            bool isGravityEnabled;
            float gravityMultiplier;

            static glm::vec2 gravity;

            static inline void ResetGravity()
            {
                gravity = L2DE_DEFAULT_GRAVITY;
            }
        };
    }
}