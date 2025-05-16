#include "CoinController.h"

#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Animator/AnimationController.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;
using namespace Learning2DEngine::Animator;


CoinController::CoinController(GameObject* gameObject, float speed)
	: Component(gameObject), speed(speed)
{
}

void CoinController::Init()
{
	gameObject->AddComponent<CircleColliderComponent>(
		gameObject->transform.GetScale().x / 2,
		ColliderType::KINEMATIC
	);

	auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
		ResourceManager::GetInstance().GetTexture(COIN_TEXTURE_IDS[0])
	);

	auto animationController = gameObject->AddComponent<AnimationController>(&renderer->data, COIN_TEXTURE_IDS.size(), true);
	animationController->speed = speed;
	for (auto& coinId : COIN_TEXTURE_IDS)
	{
		animationController->Add(std::move(AnimationFrame{
			&ResourceManager::GetInstance().GetTexture(coinId),
			0.1f
			}));
	}
	animationController->Play();
}

CoinController* CoinController::Create(glm::vec2 position, float speed)
{
	auto coin = GameObject::Create(Transform(position, COIN_SIZE));
	return coin->AddComponent<CoinController>(speed);
}