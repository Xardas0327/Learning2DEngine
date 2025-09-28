#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>

class MovingPlatformController : public Learning2DEngine::System::UpdaterComponent
{
	friend class Learning2DEngine::System::GameObject;
protected:
	glm::vec2 startPosition;
	glm::vec2 endPosition;
	glm::vec2 directionVector;
	const glm::vec2 minVector;
	const glm::vec2 maxVector;
	float speed;
	bool movingToEnd;

	MovingPlatformController(Learning2DEngine::System::GameObject* gameObject, glm::vec2 startPosition, glm::vec2 endPosition, float speed);

	void Init() override;
	void Update() override;
public:
	void Reset();

	static MovingPlatformController* Create(
		Learning2DEngine::System::GameObject* gameObject,
		glm::vec2 startPosition,
		glm::vec2 endPosition,
		float speed = 20.0f
	);
};