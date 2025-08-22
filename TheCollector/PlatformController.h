#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

#if L2DE_DEBUG
#include <Learning2DEngine/DebugTool/DebugPosition.h>
#endif

const glm::vec2 PLATFORM_SIZE(200.0f, 50.0f);
const glm::vec2 EDGE_SIZE(100.0f, 100.0f);

const glm::mat4x2 GROUND1_TEXTCOOR = glm::mat4x2{
	0.01f, 0.01f,
	0.99f, 0.01f,
	0.99f, 0.49f,
	0.01f, 0.49f
};
const glm::mat4x2 GROUND2_TEXTCOOR = glm::mat4x2{
	0.01f, 0.51f,
	0.99f, 0.51f,
	0.99f, 0.99f,
	0.01f, 0.99f
};

const glm::mat4x2 SMALL_PLATFORM_TEXTCOOR = glm::mat4x2{
	0.0f, 0.0f,
	0.33f, 0.0f,
	0.33f, 1.0f,
	0.0f, 1.0f
};

const glm::mat4x2 LARGE_PLATFORM_TEXTCOOR = glm::mat4x2{
	0.33f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.33f, 1.0f
};

class PlatformController : public Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
	std::string textureId;
	glm::mat4x2 uvMatrix;
	int layer;

	PlatformController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId, const glm::mat4x2& uvMatrix, int layer = 0)
		: Learning2DEngine::System::Component(gameObject), textureId(textureId), uvMatrix(uvMatrix), layer(layer)
	{
	}

	void Init() override
	{
		auto render = gameObject->AddComponent<Learning2DEngine::Render::SpriteRenderComponent>(
			Learning2DEngine::Render::RendererMode::RENDER,
			Learning2DEngine::System::ResourceManager::GetInstance().GetTexture(textureId),
			layer
		);
		render->data.uvMatrix = uvMatrix;

		gameObject->AddComponent<Learning2DEngine::Physics::BoxColliderComponent>(
			gameObject->transform.GetScale(),
			Learning2DEngine::Physics::ColliderType::KINEMATIC,
			Learning2DEngine::Physics::ColliderMode::COLLIDER
		);

#if L2DE_DEBUG
		gameObject->AddComponent<Learning2DEngine::DebugTool::DebugPosition>();
#endif
	}

public:
	static PlatformController* Create(
		glm::vec2 position,
		const std::string& textureId,
		const glm::mat4x2& uvMatrix,
		glm::vec2 size = PLATFORM_SIZE,
		int layer = 0)
	{
		auto platform = Learning2DEngine::System::GameObjectManager::GetInstance().CreateGameObject(
			Learning2DEngine::System::Transform(position, size)
		);
		return platform->AddComponent<PlatformController>(textureId, uvMatrix, layer);
	}
};