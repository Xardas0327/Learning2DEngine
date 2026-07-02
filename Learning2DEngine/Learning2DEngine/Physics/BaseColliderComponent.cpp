#include "BaseColliderComponent.h"

#include "Rigidbody.h"

namespace Learning2DEngine
{
	using namespace System;

    namespace Physics
    {
        BaseColliderComponent::BaseColliderComponent(
            GameObject* gameObject,
            ColliderType type,
            ColliderMode mode,
            glm::vec2 offset,
            int32_t maskLayer)
            : System::Component(gameObject), type(type), mode(mode), colliderOffset(offset), maskLayer(maskLayer), rigidbody(nullptr)
        {

        }

        void  BaseColliderComponent::InitRigidbody()
        {
            rigidbody = gameObject->GetComponent<Rigidbody>();

			L2DE_LOG_IF_ERROR(rigidbody == nullptr, "COLLIDER: The Rigidbody is not found.")
        }
    }
}