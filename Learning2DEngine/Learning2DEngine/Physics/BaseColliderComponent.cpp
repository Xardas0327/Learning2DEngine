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
            if (rigidbody == nullptr)
            {
                L2DE_LOG_ERROR("COLLIDER: The Rigidbody is not found.");
            }
        }
    }
}