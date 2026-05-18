#pragma once

#include <glm/glm.hpp>

#include "../DebugTool/DebugMacro.h"
#include "../System/UpdaterComponent.h"
#include "../System/GameObject.h"
#include "../System/Time.h"

namespace Learning2DEngine
{
    namespace Physics
    {
		// It is not recommended to use Rigidbody for children of gameobjects, which have Rigidbody,
		// because the parent will move the children by their own Rigidbody too, and it may cause unexpected behavior.
        class Rigidbody : public System::UpdaterComponent
        {
            friend class System::GameObject;
        protected:
            static constexpr const glm::vec2 DefaultGravity = glm::vec2(0.0f, 9.81f);

            Rigidbody(System::GameObject* gameObject, glm::vec2 velocity = glm::vec2(0.0f, 0.0f), bool isGravityEnabled = false, bool isFrozen = false);

            Rigidbody(System::GameObject* gameObject, bool isFrozen);
#if L2DE_DEBUG
            virtual void Init() override;
#endif // L2DE_DEBUG
            
            /// <summary>
            /// If it is not frozen, the position of gameobject will be updated.
            /// </summary>
            virtual void Update() override;
        public:
            glm::vec2 velocity;
            bool isFrozen;
            bool isGravityEnabled;
            float gravityMultiplier;

            static glm::vec2 gravity;

            static inline void ResetGravity()
            {
                gravity = Rigidbody::DefaultGravity;
            }
        };
    }
}