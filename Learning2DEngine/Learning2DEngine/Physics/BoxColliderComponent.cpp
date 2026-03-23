#include "BoxColliderComponent.h"

#if L2DE_DEBUG_SHOW_COLLIDER
#include "../DebugTool/DebugBoxColliderRenderComponent.h"
#endif
#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
    using namespace System;

    namespace Physics
    {
        BoxColliderComponent::BoxColliderComponent(
            System::GameObject* gameObject,
            glm::vec2 size,
            ColliderType type,
            ColliderMode mode,
            glm::vec2 offset,
            int32_t maskLayer)
            : System::Component(gameObject), BaseColliderComponent(gameObject, type, mode, offset, maskLayer),
            colliderSize(size)
#if L2DE_DEBUG_SHOW_COLLIDER
            , debugTool(nullptr)
#endif
        {
        }

        void BoxColliderComponent::Init()
        {
            ComponentManager::GetInstance().AddToCollider(this);

#if L2DE_DEBUG_SHOW_COLLIDER
            debugTool = gameObject->AddComponent<DebugTool::DebugBoxColliderRenderComponent>(this);
            debugTool->isActive = L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_VALUE;
#endif
        }

        void BoxColliderComponent::Destroy()
        {
            ComponentManager::GetInstance().RemoveFromCollider(this);
        }
    }
}