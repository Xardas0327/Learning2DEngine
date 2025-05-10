#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/Component.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

const glm::vec2 COIN_SIZE(50.0f, 50.0f);
const std::string COIN_TEXTURE_ID="Coin";

class CoinController : public virtual Learning2DEngine::System::Component
{
	friend class Learning2DEngine::System::GameObject;
protected:
	CoinController(Learning2DEngine::System::GameObject* gameObject)
		: Learning2DEngine::System::Component(gameObject)
	{
	}

	void Init() override
	{
		gameObject->AddComponent<Learning2DEngine::Render::SpriteRenderComponent>(
			Learning2DEngine::System::ResourceManager::GetInstance().GetTexture(COIN_TEXTURE_ID)
		);
		gameObject->AddComponent<Learning2DEngine::Physics::CircleColliderComponent>(
			gameObject->transform.GetScale().x / 2,
			Learning2DEngine::Physics::ColliderType::KINEMATIC
		);
	}

public:
	static CoinController* Create(glm::vec2 position)
	{
		auto coin = Learning2DEngine::System::GameObject::Create(
			Learning2DEngine::System::Transform(position, COIN_SIZE)
		);
		return coin->AddComponent<CoinController>();
	}
};