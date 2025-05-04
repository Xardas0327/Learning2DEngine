#include "PlayerController.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/DebugTool/Log.h>
#include <Learning2DEngine/System/InputStatus.h>
#include <Learning2DEngine/Physics/Rigidbody.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

PlayerController::PlayerController(GameObject* gameObject)
	: LateUpdaterComponent(gameObject), BaseLateUpdaterComponent(gameObject), Component(gameObject)
{
}

void PlayerController::LateUpdate()
{
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_SPACE) == InputStatus::KEY_DOWN)
    {
		gameObject->GetComponent<Rigidbody>()->velocity.y += -400.0f;
    }

    if (Game::GetKeyboardButtonStatus(GLFW_KEY_A) == InputStatus::KEY_HOLD)
    {
        gameObject->GetComponent<Rigidbody>()->velocity.x = -200.0f;
    }
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_D) == InputStatus::KEY_HOLD)
    {
        gameObject->GetComponent<Rigidbody>()->velocity.x = 200.0f;
    }
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_A) == InputStatus::KEY_UP &&
        Game::GetKeyboardButtonStatus(GLFW_KEY_D) == InputStatus::KEY_UP)
    {
        gameObject->GetComponent<Rigidbody>()->velocity.x = 0;
    }
}