#include "PowerUpController.h"

#include <Learning2DEngine/System/Game.h>

#include "PlayerController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;


PowerUpController::PowerUpController(GameObject* gameObject, const PowerUpObject& powerUpObject)
	: BoxColliderComponent(gameObject, POWERUP_SIZE), BaseBoxColliderComponent(gameObject, POWERUP_SIZE),
	BaseColliderComponent(gameObject, true, glm::vec2(0.0f, 0.0f), 0b100), Component(gameObject),
    UpdaterComponent(gameObject), BaseUpdaterComponent(gameObject), LateUpdaterComponent(gameObject), BaseLateUpdaterComponent(gameObject),
	rigidbody(nullptr), renderer(nullptr), powerUpObject(powerUpObject), actualDuration(powerUpObject.duration), activated(false),
    activationEventHandler()
{
	gameObject->transform.scale = POWERUP_SIZE;
}

void PowerUpController::Init()
{
    BoxColliderComponent::Init();
    UpdaterComponent::Init();
    LateUpdaterComponent::Init();

    rigidbody = gameObject->AddComponent<Rigidbody, glm::vec2>(VELOCITY);
    renderer = gameObject->AddComponent<SpriteRenderer, const Texture2D&, int, glm::vec3>(
        ResourceManager::GetInstance().GetTexture(powerUpObject.textureId),
        1,
        powerUpObject.color
    );

}

void PowerUpController::Destroy()
{
    BoxColliderComponent::Destroy();
    UpdaterComponent::Destroy();
    LateUpdaterComponent::Destroy();
}

void PowerUpController::OnCollision(Collision collision)
{
    if (activated)
        return;

    auto player = collision.collidedObject->GetComponent<PlayerController>();
    if (player != nullptr)
    {
        activationEventHandler.Invoke(powerUpObject.type, true);
        activated = true;

        if (actualDuration > 0.0f)
        {
            rigidbody->isActive = false;
            renderer->isActive = false;
        }
        else
            gameObject->isActive = false;

    }
}

void PowerUpController::Update()
{
    if (activated && actualDuration > 0.0f)
    {
        actualDuration -= Game::GetDeltaTime();

        if (actualDuration <= 0.0f)
        {
            activated = false;
            activationEventHandler.Invoke(powerUpObject.type, false);
            gameObject->isActive = false;
        }
    }
}

void PowerUpController::LateUpdate()
{
    if(gameObject->transform.position.y >= Game::mainCamera.GetResolution().GetHeight())
        gameObject->isActive = false;
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