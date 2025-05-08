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
#include "BushController.h"
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
    Texture2DSettings basicSettings;
    basicSettings.internalFormat = GL_RGBA;
    basicSettings.imageFormat = GL_RGBA;
    basicSettings.wrapS = GL_CLAMP_TO_EDGE;
    basicSettings.wrapT = GL_CLAMP_TO_EDGE;

    resourceManager.LoadTextureFromFile("SmallPlatform", "Assets/Images/SmallPlatform.png", basicSettings);
    resourceManager.LoadTextureFromFile("LargePlatform", "Assets/Images/LargePlatform.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin", "Assets/Images/Coin.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRight", "Assets/Images/KnightRight.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeft", "Assets/Images/KnightLeft.png", basicSettings);
    resourceManager.LoadTextureFromFile("Ground1", "Assets/Images/Ground1.png", basicSettings);
    resourceManager.LoadTextureFromFile("Ground2", "Assets/Images/Ground2.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush1", "Assets/Images/Bush1.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush2", "Assets/Images/Bush2.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush3", "Assets/Images/Bush3.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush4", "Assets/Images/Bush4.png", basicSettings);

    //Background color
    renderManager.SetClearColor(0.0f, 0.6f, 0.9f, 1.0f);

    // MSAA
    ActivateMSAA(4);

    //TEST ONLY
    //Floor
    PlatformController::Create(glm::vec2(0.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(100.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(200.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(300.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(400.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(500.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(600.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(700.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(800.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(900.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(1000.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(1100.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(1200.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(1300.0f, 650.0f), "Ground1", EDGE_SIZE);
    PlatformController::Create(glm::vec2(1400.0f, 650.0f), "Ground1", EDGE_SIZE);

    //Wall 1
    PlatformController::Create(glm::vec2(-100.0f, 650.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, 550.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, 450.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, 350.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, 250.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, 150.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, 50.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, -50.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, -150.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, -250.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, -350.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, -450.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, -550.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(-100.0f, -650.0f), "Ground2", EDGE_SIZE, 1);

    PlatformController::Create(glm::vec2(1437.5f, 650.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, 550.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, 450.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, 350.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, 250.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, 150.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, 50.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, -50.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, -150.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, -250.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, -350.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, -450.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, -550.0f), "Ground2", EDGE_SIZE, 1);
    PlatformController::Create(glm::vec2(1437.5f, -650.0f), "Ground2", EDGE_SIZE, 1);

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

    //Bush
    BushController::Create(glm::vec2(500.0f, 600.0f), "Bush2", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(550.0f, 600.0f), "Bush1", BUSH_SIZE, 1);
    BushController::Create(glm::vec2(600.0f, 600.0f), "Bush2", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(650.0f, 600.0f), "Bush1", BUSH_SIZE, 1);
    BushController::Create(glm::vec2(700.0f, 600.0f), "Bush2", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(750.0f, 600.0f), "Bush1", BUSH_SIZE, 1);
    BushController::Create(glm::vec2(1362.5f, 250.0f), "Bush3", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(1125.0f, 100.0f), "Bush4", BUSH_SIZE, 1);

    //Coin
    CoinController::Create(glm::vec2(50.0f, 75.0f));

    CoinController::Create(glm::vec2(25.0f, 525.0f));

    CoinController::Create(glm::vec2(900.0f, 500.0f));

    CoinController::Create(glm::vec2(1387.5f, 350.0f));

    CoinController::Create(glm::vec2(1125.0f, 75.0f));

    //Player
    auto player = GameObject::Create(Transform(glm::vec2(200.0f, 400.0f)));
    auto playerController = player->AddComponent<PlayerController>();

    // Camera
    Game::mainCamera.SetResolution(renderManager.GetResolution());
    auto cameraController = GameObject::Create();
    cameraController->AddComponent<CameraController>(playerController);

}