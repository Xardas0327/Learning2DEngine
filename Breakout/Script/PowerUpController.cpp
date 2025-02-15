#include "PowerUpController.h"
#include "PlayerController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;


PowerUpController::PowerUpController(GameObject* gameObject, const PowerUpObject& powerUpObject)
	: BoxColliderComponent(gameObject, POWERUP_SIZE, false),
	BaseBoxColliderComponent(gameObject, POWERUP_SIZE, false),
	BaseColliderComponent(gameObject, false), Component(gameObject),
	rigidbody(nullptr), renderer(nullptr), powerUpObject(powerUpObject), actualDuration(powerUpObject.duration), activated(false),
    activationEventHandler()
{
	gameObject->transform.scale = POWERUP_SIZE;
}

void PowerUpController::Init()
{
    BoxColliderComponent::Init();

    rigidbody = gameObject->AddComponent<Rigidbody, glm::vec2>(VELOCITY);
    renderer = gameObject->AddComponent<SpriteRenderer, const Texture2D&, int, glm::vec3>(
        ResourceManager::GetInstance().GetTexture(powerUpObject.textureId),
        1,
        powerUpObject.color
    );

}

void PowerUpController::OnCollision(Collision collision)
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
    auto powerUp = Learning2DEngine::System::GameObject::Create(
        Learning2DEngine::System::Transform(position)
    );

    PowerUpController* controller = powerUp->AddComponent<PowerUpController, const PowerUpObject&>(powerUpObject);
    controller->activationEventHandler.Add(&eventItem);

    return controller;
}