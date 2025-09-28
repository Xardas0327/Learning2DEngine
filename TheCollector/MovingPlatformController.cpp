#include "MovingPlatformController.h"

#include <Learning2DEngine/System/Time.h>

#if L2DE_DEBUG
#include <Learning2DEngine/DebugTool/DebugPosition.h>
#endif

using namespace Learning2DEngine::System;


MovingPlatformController::MovingPlatformController(GameObject* gameObject, glm::vec2 startPosition, glm::vec2 endPosition, float speed)
	: UpdaterComponent(gameObject), Component(gameObject),
	startPosition(startPosition), endPosition(endPosition),
	directionVector(glm::normalize(endPosition - startPosition)),
	minVector(startPosition.x > endPosition.x ? endPosition.x : startPosition.x, startPosition.y > endPosition.y ? endPosition.y : startPosition.y),
	maxVector(startPosition.x < endPosition.x ? endPosition.x : startPosition.x, startPosition.y < endPosition.y ? endPosition.y : startPosition.y),
	speed(speed), movingToEnd(true)
{

}

void MovingPlatformController::Init()
{
	UpdaterComponent::Init();

#if L2DE_DEBUG
	gameObject->AddComponent<Learning2DEngine::DebugTool::DebugPosition>();
#endif

	Reset();
}

void MovingPlatformController::Update()
{
	auto p = gameObject->transform.GetPosition();
	auto m = (movingToEnd ? 1.0f : -1.0f) * directionVector * speed * Time::GetDeltaTime();
	glm::vec2 newPosition = gameObject->transform.GetPosition() + (movingToEnd ? 1.0f : -1.0f) * directionVector * speed * Time::GetDeltaTime();

	gameObject->transform.SetPosition(
		glm::clamp(newPosition, minVector, maxVector)
	);

	if (glm::distance(gameObject->transform.GetPosition(), movingToEnd ? endPosition : startPosition) < 0.01f)
	{
		movingToEnd = !movingToEnd;
	}
}

void MovingPlatformController::Reset()
{
	gameObject->transform.SetPosition(startPosition);
	movingToEnd = true;
}

MovingPlatformController* MovingPlatformController::Create(
	GameObject* gameObject,
	glm::vec2 startPosition,
	glm::vec2 endPosition,
	float speed
)
{
	return gameObject->AddComponent<MovingPlatformController>(startPosition, endPosition, speed);
}