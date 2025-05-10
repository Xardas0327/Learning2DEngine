#include "PlatformDetectorController.h"

#include "PlatformController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

PlatformDetectorController::PlatformDetectorController(GameObject* gameObject, glm::vec2 size, glm::vec2 offset)
	: BoxColliderComponent(gameObject, size, ColliderType::DYNAMIC, ColliderMode::TRIGGER, offset),
    BaseBoxColliderComponent(gameObject, size, ColliderType::DYNAMIC, ColliderMode::TRIGGER, offset),
    BaseColliderComponent(gameObject, ColliderType::DYNAMIC, ColliderMode::TRIGGER, offset), Component(gameObject),
    eventhandler()
{

}

void PlatformDetectorController::Destroy()
{
    BoxColliderComponent::Destroy();
    eventhandler.Clear();
}

void PlatformDetectorController::OnCollision(const Collision& collision)
{
    auto paltform = collision.collidedObject->GetComponent<PlatformController>();
    if (paltform != nullptr)
    {
        eventhandler.Invoke();
    }
}