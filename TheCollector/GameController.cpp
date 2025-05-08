#include "GameController.h"

#include <Learning2DEngine/Render/RenderManager.h>

#include "MovingPlatformController.h"
#include "PlatformController.h"
#include "BushController.h"
#include "CameraController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

GameController::GameController(Learning2DEngine::System::GameObject* gameObject)
    : UpdaterComponent(gameObject), BaseUpdaterComponent(gameObject), Component(gameObject),
    coins(), playerController(nullptr), gameStatus(GameStatus::Menu)
{

}

void GameController::Init()
{
    UpdaterComponent::Init();

	CreateEnvironment();

    //Player
    auto player = GameObject::Create(Transform(glm::vec2(200.0f, 400.0f)));
    playerController = player->AddComponent<PlayerController>();

    // Camera
    auto cameraController = GameObject::Create();
    cameraController->AddComponent<CameraController>(playerController);

    StartPlay();
}

void GameController::CreateEnvironment()
{
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

    //Wall
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
    coins.reserve(5);
    coins.push_back(CoinController::Create(glm::vec2(50.0f, 75.0f)));
    coins.push_back(CoinController::Create(glm::vec2(25.0f, 525.0f)));
    coins.push_back(CoinController::Create(glm::vec2(900.0f, 500.0f)));
    coins.push_back(CoinController::Create(glm::vec2(1387.5f, 350.0f)));
    coins.push_back(CoinController::Create(glm::vec2(1125.0f, 75.0f)));

}

void GameController::StartPlay()
{
    for (auto& coin : coins)
    {
        coin->gameObject->isActive = true;
    }

    playerController->gameObject->transform.SetPosition(glm::vec2(200.0f, 400.0f));
    playerController->rigidbody->isFrozen = false;
    playerController->coinNumber = 0;

    gameStatus = GameStatus::Play;
}

void GameController::EndPlay()
{
    playerController->rigidbody->isFrozen = true;

    gameStatus = GameStatus::Ended;
}

void GameController::Update()
{
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_ESCAPE) == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    // If the player collected all coins
    if (gameStatus == GameStatus::Play && playerController->coinNumber == coins.size())
        EndPlay();
}