#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

const glm::vec2 BUSH_SIZE(50.0f, 50.0f);
const glm::mat4x2 BUSH1_TEXTCOOR = glm::mat4x2{
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 0.25f,
	0.0f, 0.25f
};
const glm::mat4x2 BUSH2_TEXTCOOR = glm::mat4x2{
	0.0f, 0.26f,
	1.0f, 0.26f,
	1.0f, 0.5f,
	0.0f, 0.5f
};
const glm::mat4x2 BUSH3_TEXTCOOR = glm::mat4x2{
	0.0f, 0.51f,
	1.0f, 0.51f,
	1.0f, 0.75f,
	0.0f, 0.75f
};
const glm::mat4x2 BUSH4_TEXTCOOR = glm::mat4x2{
	0.0f, 0.76f,
	1.0f, 0.76f,
	1.0f, 1.0f,
	0.0f, 1.0f
};

class BushController : public Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
	std::string textureId;
	glm::mat4x2 uvMatrix;
	int layer;

	BushController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId, const glm::mat4x2& uvMatrix, int layer = 0)
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
	}

public:
	static BushController* Create(
		glm::vec2 position,
		const std::string& textureId,
		const glm::mat4x2& uvMatrix,
		glm::vec2 size = BUSH_SIZE,
		int layer = 0
	)
	{
		auto platform = Learning2DEngine::System::GameObjectManager::GetInstance().CreateGameObject(
			Learning2DEngine::System::Transform(position, size)
		);
		return platform->AddComponent<BushController>(textureId, uvMatrix, layer);
	}
};