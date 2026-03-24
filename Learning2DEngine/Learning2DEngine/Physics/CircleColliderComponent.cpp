#include "CircleColliderComponent.h"

#if L2DE_DEBUG_SHOW_COLLIDER
#include "../DebugTool/DebugCircleColliderRenderComponent.h"
#endif
#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
    using namespace System;

    namespace Physics
    {
        CircleColliderComponent::CircleColliderComponent(
            System::GameObject* gameObject,
            float radius,
            ColliderType type,
            ColliderMode mode,
            glm::vec2 offset,
            int32_t maskLayer)
            : System::Component(gameObject), BaseColliderComponent(gameObject, type, mode, offset, maskLayer),
            colliderRadius(radius)
#if L2DE_DEBUG_SHOW_COLLIDER
            , debugTool(nullptr)
#endif
        {

        }
        void CircleColliderComponent::Init()
        {
            System::ComponentManager::GetInstance().AddToCollider(this);
#if L2DE_DEBUG_SHOW_COLLIDER
            debugTool = gameObject->AddComponent<DebugTool::DebugCircleColliderRenderComponent>(this);
            debugTool->isActive = L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_VALUE;
#endif
        }
        void CircleColliderComponent::Destroy()
        {
            System::ComponentManager::GetInstance().RemoveFromCollider(this);
        }
    }
}