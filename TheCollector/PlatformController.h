#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

const glm::vec2 PLATFORM_SIZE(200.0f, 50.0f);

class PlatformController : public virtual Learning2DEngine::System::Component
{
    friend class Learning2DEngine::System::GameObject;
protected:
	std::string textureId;

    PlatformController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId)
		: Learning2DEngine::System::Component(gameObject), textureId(textureId)
	{
	}

	void Init() override
	{
		gameObject->AddComponent<Learning2DEngine::Render::SpriteRenderComponent>(
			Learning2DEngine::System::ResourceManager::GetInstance().GetTexture(textureId)
		);
		gameObject->AddComponent<Learning2DEngine::Physics::BoxColliderComponent>(
			gameObject->transform.GetScale(),
			Learning2DEngine::Physics::ColliderType::KINEMATIC,
			Learning2DEngine::Physics::ColliderMode::COLLIDER
		);
	}

public:
	static PlatformController* Create(glm::vec2 position, const std::string& textureId, glm::vec2 size = PLATFORM_SIZE)
	{
		auto platform = Learning2DEngine::System::GameObject::Create(
			Learning2DEngine::System::Transform(position, size)
		);
		return platform->AddComponent<PlatformController>(textureId);
	}
};