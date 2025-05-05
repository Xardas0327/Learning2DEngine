#include "TheCollector.h"

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>

#include "PlayerController.h"
#include "PlatformController.h"
#include "MovingPlatformController.h"

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

void TheCollector::Init()
{
    Game::Init();

    auto& renderManager = RenderManager::GetInstance();

    //Background color
    renderManager.SetClearColor(0.0f, 0.6f, 0.9f, 1.0f);

    // MSAA
    ActivateMSAA(4);

    // Camera
    Game::mainCamera.SetResolution(renderManager.GetResolution());

    //TEST ONLY
    //Floor
    auto floor = GameObject::Create(
        Transform(glm::vec2(0.0f, 650.0f), glm::vec2(1440.0f, 100.0f))
    );
    floor->AddComponent<SpriteRenderComponent>(0, glm::vec4(0.72f, 0.48f, 0.34f, 1.0f));
    floor->AddComponent<BoxColliderComponent>(floor->transform.GetScale(), ColliderType::KINEMATIC, ColliderMode::COLLIDER);

    //Wall 1
    auto wall1 = GameObject::Create(
        Transform(glm::vec2(-650.0f, 100.0f), glm::vec2(1200.0f, 100.0f), 90)
    );
    wall1->AddComponent<SpriteRenderComponent>(0, glm::vec4(0.72f, 0.48f, 0.34f, 1.0f));
    wall1->AddComponent<BoxColliderComponent>(
        glm::vec2(wall1->transform.GetScale().y, wall1->transform.GetScale().x),
        ColliderType::KINEMATIC,
        ColliderMode::COLLIDER,
        glm::vec2(550.0f, -550.0f)
    );

    //Wall 2
    auto wall2 = GameObject::Create(
        Transform(glm::vec2(890.0f, 100.0f), glm::vec2(1200.0f, 100.0f), 90)
    );
    wall2->AddComponent<SpriteRenderComponent>(0, glm::vec4(0.72f, 0.48f, 0.34f, 1.0f));
    wall2->AddComponent<BoxColliderComponent>(
        glm::vec2(wall2->transform.GetScale().y, wall2->transform.GetScale().x),
        ColliderType::KINEMATIC,
        ColliderMode::COLLIDER,
        glm::vec2(550.0f, -550.0f)
    );

    //Platform 1
    PlatformController::Create(glm::vec2(200.0f, 500.0f));

    //Platform 2
    PlatformController::Create(glm::vec2(000.0f, 150.0f));

    //Moving Platform 1
    MovingPlatformController::Create(glm::vec2(500.0f, 100.0f), glm::vec2(500.0f, 500.0f));

    //Player
    auto player = GameObject::Create(Transform(glm::vec2(200.0f, 400.0f)));
    player->AddComponent<PlayerController>();

}