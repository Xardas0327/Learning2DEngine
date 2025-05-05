#include "PlayerController.h"

#include <Learning2DEngine/DebugTool/Log.h>
#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/InputStatus.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;

PlayerController::PlayerController(GameObject* gameObject)
	: LateUpdaterComponent(gameObject), BaseLateUpdaterComponent(gameObject), Component(gameObject),
	cameraDistanceX(0.0f), rigidbody(nullptr)
{
}

void PlayerController::Init()
{
    LateUpdaterComponent::Init();

    cameraDistanceX = Game::mainCamera.GetPosition().x - gameObject->transform.GetPosition().x;
    gameObject->AddComponent<SpriteRenderComponent>();
    rigidbody = gameObject->AddComponent<Rigidbody>(glm::vec2(0.0f, 0.0f), true);
    rigidbody->gravityMultiplier = 50.0f;

    auto collider = gameObject->AddComponent<BoxColliderComponent>(gameObject->transform.GetScale(), ColliderType::DYNAMIC, ColliderMode::COLLIDER);
    collider->InitRigidbody();
}

void PlayerController::LateUpdate()
{
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_SPACE) == InputStatus::KEY_DOWN)
    {
        rigidbody->velocity.y += -400.0f;
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