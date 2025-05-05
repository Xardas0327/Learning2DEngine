#include "PlayerController.h"

#include <Learning2DEngine/DebugTool/Log.h>
#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/InputStatus.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;

PlayerController::PlayerController(GameObject* gameObject)
    : LateUpdaterComponent(gameObject), BaseLateUpdaterComponent(gameObject), Component(gameObject),
    BoxColliderComponent(gameObject, glm::vec2(30.0f, 5.0f), ColliderType::DYNAMIC, ColliderMode::TRIGGER, glm::vec2(10.0f, 45.0f), 0b01),
    BaseBoxColliderComponent(gameObject, glm::vec2(30.0f, 5.0f), ColliderType::DYNAMIC, ColliderMode::TRIGGER, glm::vec2(10.0f, 45.0f), 0b01),
    BaseColliderComponent(gameObject, ColliderType::DYNAMIC, ColliderMode::TRIGGER, glm::vec2(10.0f, 45.0f), 0b1),
    cameraDistanceX(0.0f), onGround(true), rigidbody(nullptr)
{
}

void PlayerController::Init()
{
    LateUpdaterComponent::Init();
    BoxColliderComponent::Init();

    gameObject->transform.SetScale(PLAYER_SIZE);

    cameraDistanceX = -615.0f;
    gameObject->AddComponent<SpriteRenderComponent>();
    rigidbody = gameObject->AddComponent<Rigidbody>(glm::vec2(0.0f, 0.0f), true);
    rigidbody->gravityMultiplier = 50.0f;

    auto collider = gameObject->AddComponent<BoxColliderComponent>(
        gameObject->transform.GetScale(),
        ColliderType::DYNAMIC,
        ColliderMode::COLLIDER,
        glm::vec2(0.0f, 0.0f),
        0b10
    );
    collider->InitRigidbody();
}

void PlayerController::Destroy()
{
    LateUpdaterComponent::Destroy();
    BoxColliderComponent::Destroy();
}

void PlayerController::LateUpdate()
{
    if (onGround && Game::GetKeyboardButtonStatus(GLFW_KEY_SPACE) == InputStatus::KEY_DOWN)
    {
        rigidbody->velocity.y += -400.0f;
        onGround = false;
    }

    if (Game::GetKeyboardButtonStatus(GLFW_KEY_A) == InputStatus::KEY_HOLD)
    {
        rigidbody->velocity.x = -200.0f;
    }
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_D) == InputStatus::KEY_HOLD)
    {
        rigidbody->velocity.x = 200.0f;
    }
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_A) == InputStatus::KEY_UP &&
        Game::GetKeyboardButtonStatus(GLFW_KEY_D) == InputStatus::KEY_UP)
    {
        rigidbody->velocity.x = 0;
    }

    Game::mainCamera.SetPosition(glm::vec2(
        gameObject->transform.GetPosition().x + cameraDistanceX,
        Game::mainCamera.GetPosition().y
    ));
}

void PlayerController::OnCollision(const Collision& collision)
{
    onGround = true;
}