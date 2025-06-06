#include "PowerUpController.h"
#include "PlayerController.h"

#include <Learning2DEngine/System/GameObjectManager.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;


PowerUpController::PowerUpController(GameObject* gameObject, const PowerUpObject& powerUpObject)
	: BoxColliderComponent(gameObject, POWERUP_SIZE, ColliderType::KINEMATIC, ColliderMode::TRIGGER, glm::vec2(0.0f, 0.0f), 0b100),
    Component(gameObject),
	rigidbody(nullptr), renderer(nullptr), powerUpObject(powerUpObject), actualDuration(powerUpObject.duration), activated(false),
    activationEventHandler()
{
    gameObject->transform.SetScale(POWERUP_SIZE);
}

void PowerUpController::Init()
{
    BoxColliderComponent::Init();

    rigidbody = gameObject->AddComponent<Rigidbody>(VELOCITY);
    renderer = gameObject->AddComponent<SpriteRenderComponent>(
        RendererMode::RENDER,
        ResourceManager::GetInstance().GetTexture(powerUpObject.textureId),
        1,
        powerUpObject.color
    );

}

void PowerUpController::OnCollision(const Collision& collision)
{
    auto player = collision.collidedObject->GetComponent<PlayerController>();
    if (player != nullptr)
    {
        activationEventHandler.Invoke(powerUpObject.type);
        activated = true;
        gameObject->isActive = false;
    }
}

PowerUpController* PowerUpController::CreatePowerUp(
    const PowerUpObject& powerUpObject,
    glm::vec2 position, 
    PowerUpActivationEventItem& eventItem)
{
    auto powerUp = GameObjectManager::GetInstance().CreateGameObject(
        Transform(position)
    );

    PowerUpController* controller = powerUp->AddComponent<PowerUpController>(powerUpObject);
    controller->activationEventHandler.Add(&eventItem);

    return controller;
}