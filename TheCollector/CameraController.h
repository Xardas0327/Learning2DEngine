#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/LateUpdaterComponent.h>

#include "PlayerController.h"

const glm::vec2 MIN_CAMERA_POSITIONS(0.0f, 0.0f);
const glm::vec2 MAX_CAMERA_POSITIONS(72.0f, 200.0f);
const float PLAYER_DISTANCEX = -100.0f;

class CameraController : public Learning2DEngine::System::LateUpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
	const PlayerController* playerController;

    CameraController(Learning2DEngine::System::GameObject* gameObject, PlayerController* playerController)
        : LateUpdaterComponent(gameObject), Component(gameObject),
		playerController(playerController)
    {

    }

    void LateUpdate() override
    {
        Learning2DEngine::System::Game::mainCamera.SetPosition(
            glm::clamp(
                glm::vec2(
                    playerController->gameObject->transform.GetPosition().x + PLAYER_DISTANCEX,
                    Learning2DEngine::System::Game::mainCamera.GetPosition().y
                ),
                MIN_CAMERA_POSITIONS,
                MAX_CAMERA_POSITIONS
            )
        );
    }
};