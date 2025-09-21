#include "PlatformDetectorController.h"

#include <Learning2DEngine/System/PropertyComponent.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

std::string ResetJump = "ResetJump";

PlatformDetectorController::PlatformDetectorController(GameObject* gameObject, glm::vec2 size, glm::vec2 offset)
	: BoxColliderComponent(gameObject, size, ColliderType::DYNAMIC, ColliderMode::TRIGGER, offset),
    Component(gameObject), eventhandler()
{

}

void PlatformDetectorController::Destroy()
{
    BoxColliderComponent::Destroy();
    eventhandler.Clear();
}

void PlatformDetectorController::OnCollision(const Collision& collision)
{
    auto propertyComponent = collision.collidedObject->GetComponent<PropertyComponent>();
    if (propertyComponent != nullptr && propertyComponent->properties[ResetJump].GetBool())
    {
        eventhandler.Invoke();
    }
}