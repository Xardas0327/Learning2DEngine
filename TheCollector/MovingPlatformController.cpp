#include "MovingPlatformController.h"

#include <Learning2DEngine/System/Game.h>

#include "PlatformController.h"

using namespace Learning2DEngine::System;


MovingPlatformController::MovingPlatformController(GameObject* gameObject, glm::vec2 endPosition, float speed)
	: UpdaterComponent(gameObject), BaseUpdaterComponent(gameObject), Component(gameObject),
	startPosition(gameObject->transform.GetPosition()), endPosition(endPosition),
	directionVector(glm::normalize(endPosition - gameObject->transform.GetPosition())),
	speed(speed), movingToEnd(true)
{

}

void MovingPlatformController::Update()
{
	if (movingToEnd)
	{
		gameObject->transform.SetPosition(
			gameObject->transform.GetPosition() + directionVector * speed * Game::GetDeltaTime()
		);
		if (glm::distance(gameObject->transform.GetPosition(), endPosition) < 1.0f)
		{
			movingToEnd = false;
		}
	}
	else
	{
		gameObject->transform.SetPosition(
			gameObject->transform.GetPosition() - directionVector * speed * Game::GetDeltaTime()
		);
		if (glm::distance(gameObject->transform.GetPosition(), startPosition) < 1.0f)
		{
			movingToEnd = true;
		}
	}
}

MovingPlatformController* MovingPlatformController::Create(glm::vec2 startPosition, glm::vec2 endPosition, float speed)
{
	auto platform = PlatformController::Create(startPosition);

	return platform->gameObject->AddComponent<MovingPlatformController>(endPosition, speed);
}