#include "TheCollector.h"

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>

#include "CameraController.h"
#include "CoinController.h"
#include "MovingPlatformController.h"
#include "PlatformController.h"
#include "PlayerController.h"

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

void TheCollector::Init()
{
    Game::Init();

    auto& renderManager = RenderManager::GetInstance();
    auto& resourceManager = ResourceManager::GetInstance();

    // Textures
    Texture2DSettings alphaSettings;
    alphaSettings.internalFormat = GL_RGBA;
    alphaSettings.imageFormat = GL_RGBA;

    resourceManager.LoadTextureFromFile("SmallPlatform", "Assets/Images/SmallPlatform.png", alphaSettings);
    resourceManager.LoadTextureFromFile("LargePlatform", "Assets/Images/LargePlatform.png", alphaSettings);
    resourceManager.LoadTextureFromFile("Coin", "Assets/Images/Coin.png", alphaSettings);

    //Background color
    renderManager.SetClearColor(0.0f, 0.6f, 0.9f, 1.0f);

    // MSAA
    ActivateMSAA(4);

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
    wall1->AddComponent<SpriteRenderComponent>(1, glm::vec4(0.72f, 0.48f, 0.34f, 1.0f));
    wall1->AddComponent<BoxColliderComponent>(
        glm::vec2(wall1->transform.GetScale().y, wall1->transform.GetScale().x),
        ColliderType::KINEMATIC,
        ColliderMode::COLLIDER,
        glm::vec2(550.0f, -550.0f)
    );

    auto wall2 = GameObject::Create(
        Transform(glm::vec2(890.0f, 100.0f), glm::vec2(1200.0f, 100.0f), 90)
    );
    wall2->AddComponent<SpriteRenderComponent>(1, glm::vec4(0.72f, 0.48f, 0.34f, 1.0f));
    wall2->AddComponent<BoxColliderComponent>(
        glm::vec2(wall2->transform.GetScale().y, wall2->transform.GetScale().x),
        ColliderType::KINEMATIC,
        ColliderMode::COLLIDER,
        glm::vec2(550.0f, -550.0f)
    );

    //Platform
    PlatformController::Create(glm::vec2(200.0f, 500.0f), "LargePlatform");

    PlatformController::Create(glm::vec2(0.0f, 150.0f), "LargePlatform");

    PlatformController::Create(glm::vec2(850.0f, 500.0f), "SmallPlatform", glm::vec2(PLATFORM_SIZE.x / 4, PLATFORM_SIZE.y));

    PlatformController::Create(glm::vec2(900.0f, 450.0f), "SmallPlatform", glm::vec2(PLATFORM_SIZE.x / 4, PLATFORM_SIZE.y));

    PlatformController::Create(glm::vec2(1340.0f, 300.0f), "SmallPlatform", glm::vec2(PLATFORM_SIZE.x / 2, PLATFORM_SIZE.y));

    PlatformController::Create(glm::vec2(1100.0f, 150.0f), "SmallPlatform", glm::vec2(PLATFORM_SIZE.x / 2, PLATFORM_SIZE.y));

    //Moving Platform
    MovingPlatformController::Create(glm::vec2(500.0f, 150.0f), glm::vec2(500.0f, 500.0f), "LargePlatform");

    MovingPlatformController::Create(glm::vec2(950.0f, 450.0f), glm::vec2(1150.0f, 450.0f), "LargePlatform");

    //Coin
    CoinController::Create(glm::vec2(50.0f, 75.0f), "Coin");

    CoinController::Create(glm::vec2(25.0f, 525.0f), "Coin");

    CoinController::Create(glm::vec2(900.0f, 500.0f), "Coin");

    CoinController::Create(glm::vec2(1387.5f, 350.0f), "Coin");

    CoinController::Create(glm::vec2(1125.0f, 75.0f), "Coin");

    //Player
    auto player = GameObject::Create(Transform(glm::vec2(200.0f, 400.0f)));
    auto playerController = player->AddComponent<PlayerController>();

    // Camera
    Game::mainCamera.SetResolution(renderManager.GetResolution());
    auto cameraController = GameObject::Create();
    cameraController->AddComponent<CameraController>(playerController);

}