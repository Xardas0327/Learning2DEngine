#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

class MovingPlatformController : public virtual Learning2DEngine::System::UpdaterComponent
{
	friend class Learning2DEngine::System::GameObject;
protected:
	glm::vec2 startPosition;
	glm::vec2 endPosition;
	glm::vec2 directionVector;
	float speed;
	bool movingToEnd;

	MovingPlatformController(Learning2DEngine::System::GameObject* gameObject, glm::vec2 endPosition, float speed);

	void Update() override;
public:
	static MovingPlatformController* Create(glm::vec2 startPosition, glm::vec2 endPosition, float speed = 100.0f);
};