#include "PlatformDetectorController.h"

#include "PlatformController.h"
#include "BoxController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

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
    auto paltform = collision.collidedObject->GetComponent<PlatformController>();
    auto box = collision.collidedObject->GetComponent<BoxController>();
    if (paltform != nullptr || box != nullptr)
    {
        eventhandler.Invoke();
    }
}