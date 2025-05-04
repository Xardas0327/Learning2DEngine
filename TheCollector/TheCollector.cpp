#include "TheCollector.h"

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Physics/Rigidbody.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>

#include "PlayerController.h"

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

void TheCollector::Init()
{
	Game::Init();

    // MSAA
    ActivateMSAA(4);

    // Camera
    Game::mainCamera.SetResolution(RenderManager::GetInstance().GetResolution());

    //TEST ONLY
    //Floor
    auto floor = GameObject::Create(
		Transform(glm::vec2(320.0f, 600.0f), glm::vec2(600.0f, 100.0f))
    );
    floor->AddComponent<SpriteRenderComponent>(0, glm::vec4(0.72f, 0.48f, 0.34f, 1.0f));
    floor->AddComponent<BoxColliderComponent>(floor->transform.GetScale(), ColliderType::KINEMATIC, ColliderMode::COLLIDER);

    //Player
    auto player = GameObject::Create(
        Transform(glm::vec2(615.0f, 400.0f), glm::vec2(50.0f, 50.0f))
    );
    player->AddComponent<SpriteRenderComponent>(0, glm::vec4(1.0f));
    auto playerRigidbody = player->AddComponent<Rigidbody>(glm::vec2(0.0f, 0.0f), true);
    playerRigidbody->gravityMultiplier = 50.0f;
    auto playerCollider = player->AddComponent<BoxColliderComponent>(player->transform.GetScale(), ColliderType::DYNAMIC, ColliderMode::COLLIDER);
    playerCollider->InitRigidbody();
    player->AddComponent<PlayerController>();

}