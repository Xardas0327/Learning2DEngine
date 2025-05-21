#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

#include "PlatformController.h"

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

	MovingPlatformController(Learning2DEngine::System::GameObject* gameObject, glm::vec2 endPosition, float speed);

	void Update() override;
public:
	void Reset();

	static MovingPlatformController* Create(
		glm::vec2 startPosition,
		glm::vec2 endPosition,
		const std::string& textureId,
		float speed = 100.0f,
		glm::vec2 size = PLATFORM_SIZE);
};