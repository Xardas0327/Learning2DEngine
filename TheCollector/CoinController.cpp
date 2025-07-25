#include "CoinController.h"

#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Animator/AnimationController.h>

#if L2DE_DEBUG
#include <Learning2DEngine/DebugTool/DebugPosition.h>
#endif

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;
using namespace Learning2DEngine::Animator;
using namespace Learning2DEngine::DebugTool;


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

#if L2DE_DEBUG
	gameObject->AddComponent<DebugPosition>();
#endif

	auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
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
	auto coin = GameObjectManager::GetInstance().CreateGameObject(Transform(position, COIN_SIZE));
	return coin->AddComponent<CoinController>(speed);
}