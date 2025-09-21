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
		gameObject->transform.GetScale().x / 2.0f,
		ColliderType::KINEMATIC
	);
	
#if L2DE_DEBUG
	gameObject->AddComponent<DebugPosition>();
#endif

	auto renderer = gameObject->AddComponent<SpriteRenderComponent>(RendererMode::RENDER);

	auto animationController = gameObject->AddComponent<AnimationController>(&renderer->data, COIN_ANIMATION_NUMBER, true);
	animationController->speed = speed;
	
	for(int i = 0; i < COIN_ANIMATION_NUMBER; ++i)
	{
		animationController->Add(AnimationFrame{
			&ResourceManager::GetInstance().GetTexture(COIN_TEXTURE_ID),
			glm::mat4x2 {
				i / 12.0f, 0.0f,
				(i + 1.0f) / 12.0f, 0.0f,
				(i + 1.0f) / 12.0f, 1.0f,
				i / 12.0f, 1.0f
			},
			0.1f
			});
	}

	animationController->Play();
}

CoinController* CoinController::Create(glm::vec2 position, float speed)
{
	auto coin = GameObjectManager::GetInstance().CreateGameObject(Transform(position, COIN_SIZE));
	return coin->AddComponent<CoinController>(speed);
}