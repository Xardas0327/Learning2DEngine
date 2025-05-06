#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

const glm::vec2 COIN_SIZE(50.0f, 50.0f);

class CoinController : public virtual Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
	std::string textureId;

	CoinController(Learning2DEngine::System::GameObject* gameObject, const std::string& textureId)
		: Learning2DEngine::System::Component(gameObject), textureId(textureId)
	{
	}

	void Init() override
	{
		gameObject->AddComponent<Learning2DEngine::Render::SpriteRenderComponent>(
			Learning2DEngine::System::ResourceManager::GetInstance().GetTexture(textureId)
		);
		gameObject->AddComponent<Learning2DEngine::Physics::CircleColliderComponent>(
			gameObject->transform.GetScale().x / 2,
			Learning2DEngine::Physics::ColliderType::KINEMATIC
		);
	}

public:
	static CoinController* Create(glm::vec2 position, const std::string& textureId)
	{
		auto coin = Learning2DEngine::System::GameObject::Create(
			Learning2DEngine::System::Transform(position, COIN_SIZE)
		);
		return coin->AddComponent<CoinController>(textureId);
	}
};