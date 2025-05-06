#include "MovingPlatformController.h"

#include <Learning2DEngine/System/Game.h>

using namespace Learning2DEngine::System;


MovingPlatformController::MovingPlatformController(GameObject* gameObject, glm::vec2 endPosition, float speed)
	: UpdaterComponent(gameObject), BaseUpdaterComponent(gameObject), Component(gameObject),
	startPosition(gameObject->transform.GetPosition()), endPosition(endPosition),
	directionVector(glm::normalize(endPosition - gameObject->transform.GetPosition())),
	minVector(startPosition.x > endPosition.x ? endPosition.x : startPosition.x, startPosition.y > endPosition.y ? endPosition.y : startPosition.y),
	maxVector(startPosition.x < endPosition.x ? endPosition.x : startPosition.x, startPosition.y < endPosition.y ? endPosition.y : startPosition.y),
	speed(speed), movingToEnd(true)
{

}

void MovingPlatformController::Update()
{
	glm::vec2 newPosition = gameObject->transform.GetPosition() + (movingToEnd ? 1.0f : -1.0f) * directionVector * speed * Game::GetDeltaTime();

	gameObject->transform.SetPosition(
		glm::clamp(newPosition, minVector, maxVector)
	);

	if (glm::distance(gameObject->transform.GetPosition(), movingToEnd ? endPosition : startPosition) < 0.00001f)
	{
		movingToEnd = !movingToEnd;
	}
}

MovingPlatformController* MovingPlatformController::Create(
	glm::vec2 startPosition,
	glm::vec2 endPosition,
	const std::string& textureId,
	float speed,
	glm::vec2 size)
{
	auto platform = PlatformController::Create(startPosition, textureId, size);

	return platform->gameObject->AddComponent<MovingPlatformController>(endPosition, speed);
}