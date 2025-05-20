#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

const glm::vec2 BUSH_SIZE(50.0f, 50.0f);

class BushController : public virtual Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
	std::string textureId;
	int layer;

	BushController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId, int layer = 0)
		: Learning2DEngine::System::Component(gameObject), textureId(textureId), layer(layer)
	{
	}

	void Init() override
	{
		gameObject->AddComponent<Learning2DEngine::Render::SpriteRenderComponent, const Learning2DEngine::Render::Texture2D&, int>(
			Learning2DEngine::System::ResourceManager::GetInstance().GetTexture(textureId),
			layer
		);
	}

public:
	static BushController* Create(glm::vec2 position, const std::string& textureId, glm::vec2 size = BUSH_SIZE, int layer = 0)
	{
		auto platform = Learning2DEngine::System::GameObject::Create(
			Learning2DEngine::System::Transform(position, size)
		);
		return platform->AddComponent<BushController, const std::string&, int>(textureId, layer);
	}
};