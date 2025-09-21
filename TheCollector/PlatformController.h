#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/PropertyComponent.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

#if L2DE_DEBUG
#include <Learning2DEngine/DebugTool/DebugPosition.h>
#endif

const glm::vec2 PLATFORM_SIZE(32.0f, 9.0f);

class PlatformController : public Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
	std::string textureId;
	int layer;

	PlatformController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId, int layer = 0)
		: Learning2DEngine::System::Component(gameObject), textureId(textureId), layer(layer)
	{
	}

	void Init() override
	{
		auto render = gameObject->AddComponent<Learning2DEngine::Render::SpriteRenderComponent>(
			Learning2DEngine::Render::RendererMode::RENDER,
			Learning2DEngine::System::ResourceManager::GetInstance().GetTexture(textureId),
			layer
		);

		gameObject->AddComponent<Learning2DEngine::Physics::BoxColliderComponent>(
			gameObject->transform.GetScale(),
			Learning2DEngine::Physics::ColliderType::KINEMATIC,
			Learning2DEngine::Physics::ColliderMode::COLLIDER
		);

		auto propertyComponent = gameObject->AddComponent<Learning2DEngine::System::PropertyComponent>();
		propertyComponent->properties.emplace("ResetJump", Learning2DEngine::System::Property(true));

#if L2DE_DEBUG
		gameObject->AddComponent<Learning2DEngine::DebugTool::DebugPosition>();
#endif
	}

public:
	static PlatformController* Create(
		glm::vec2 position,
		const std::string& textureId,
		glm::vec2 size = PLATFORM_SIZE,
		int layer = 0)
	{
		auto platform = Learning2DEngine::System::GameObjectManager::GetInstance().CreateGameObject(
			Learning2DEngine::System::Transform(position, size)
		);
		return platform->AddComponent<PlatformController>(textureId, layer);
	}
};