#include "PlayerController.h"

#include <Learning2DEngine/DebugTool/Log.h>
#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/InputStatus.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;

PlayerController::PlayerController(GameObject* gameObject)
    : UpdaterComponent(gameObject), BaseUpdaterComponent(gameObject), Component(gameObject),
    BoxColliderComponent(gameObject, glm::vec2(30.0f, 5.0f), ColliderType::DYNAMIC, ColliderMode::TRIGGER, glm::vec2(10.0f, 45.0f), 0b01),
    BaseBoxColliderComponent(gameObject, glm::vec2(30.0f, 5.0f), ColliderType::DYNAMIC, ColliderMode::TRIGGER, glm::vec2(10.0f, 45.0f), 0b01),
    BaseColliderComponent(gameObject, ColliderType::DYNAMIC, ColliderMode::TRIGGER, glm::vec2(10.0f, 45.0f), 0b1),
    onGround(true), rigidbody(nullptr)
{
}

void PlayerController::Init()
{
    UpdaterComponent::Init();
    BoxColliderComponent::Init();

    gameObject->transform.SetScale(PLAYER_SIZE);

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
    UpdaterComponent::Destroy();
    BoxColliderComponent::Destroy();
}

void PlayerController::Update()
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
    else if (Game::GetKeyboardButtonStatus(GLFW_KEY_D) == InputStatus::KEY_HOLD)
    {
        rigidbody->velocity.x = 200.0f;
    }
    else
    {
        rigidbody->velocity.x = 0;
    }
}

void PlayerController::OnCollision(const Collision& collision)
{
    onGround = true;
}