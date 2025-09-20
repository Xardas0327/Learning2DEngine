#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/PropertyComponent.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Physics/Rigidbody.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

#include "PlatformController.h"

#if L2DE_DEBUG
#include <Learning2DEngine/DebugTool/DebugPosition.h>
#endif

const glm::vec2 BOX_SIZE(10.0f, 10.0f);

class BoxController : public Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
	glm::vec2 startPostion;
	int layer;
	Learning2DEngine::Physics::Rigidbody* rigidbody;

	BoxController(Learning2DEngine::System::GameObject* gameObject, int layer = 0)
		: Learning2DEngine::System::Component(gameObject), layer(layer), startPostion(gameObject->transform.GetPosition()),
		rigidbody(nullptr)
	{
	}

	void Init() override
	{
		auto render = gameObject->AddComponent<Learning2DEngine::Render::SpriteRenderComponent>(
			Learning2DEngine::Render::RendererMode::RENDER,
			Learning2DEngine::System::ResourceManager::GetInstance().GetTexture("Grounds"),
			layer
		);
		render->data.uvMatrix = GROUND2_TEXTCOOR;

		rigidbody = gameObject->AddComponent<Learning2DEngine::Physics::Rigidbody>(glm::vec2(0.0f, 0.0f), true);
		rigidbody->gravityMultiplier = 100.0f;

		auto collider = gameObject->AddComponent<Learning2DEngine::Physics::BoxColliderComponent>(
			gameObject->transform.GetScale(),
			Learning2DEngine::Physics::ColliderType::DYNAMIC,
			Learning2DEngine::Physics::ColliderMode::COLLIDER
		);
		collider->InitRigidbody();

		auto propertyComponent = gameObject->AddComponent<Learning2DEngine::System::PropertyComponent>();
		propertyComponent->properties.emplace("ResetJump", Learning2DEngine::System::Property(true));

#if L2DE_DEBUG
		gameObject->AddComponent<Learning2DEngine::DebugTool::DebugPosition>();
#endif
	}

public:
	void ResetPosition()
	{
		gameObject->transform.SetPosition(startPostion);
		rigidbody->velocity = glm::vec2(0.0f, 0.0f);
	}

	static BoxController* Create(
		glm::vec2 position,
		int layer = 0)
	{
		auto gameObject = Learning2DEngine::System::GameObjectManager::GetInstance().CreateGameObject(
			Learning2DEngine::System::Transform(position, BOX_SIZE)
		);
		return gameObject->AddComponent<BoxController>(layer);
	}
};