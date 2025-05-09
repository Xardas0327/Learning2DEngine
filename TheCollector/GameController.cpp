#include "GameController.h"

#include <format>

#include <Learning2DEngine/DebugTool/DebugMacro.h>
#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/Object/FpsShower.h>

#include "MovingPlatformController.h"
#include "PlatformController.h"
#include "BushController.h"
#include "CameraController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Object;
using namespace Learning2DEngine::UI;

GameController::GameController(Learning2DEngine::System::GameObject* gameObject)
    : UpdaterComponent(gameObject), BaseUpdaterComponent(gameObject), Component(gameObject),
    coins(), playerController(nullptr), gameStatus(GameStatus::Menu), fontSizePair("Assets/Fonts/PixelOperator8.ttf", 24),
    scoreText(nullptr), playTimeText(nullptr), playerCoinEventItem(this), currentPlayTime(0)
{

}

void GameController::Init()
{
    UpdaterComponent::Init();

	CreateEnvironment();

    //Player
    auto player = GameObject::Create(Transform(glm::vec2(200.0f, 400.0f)));
    playerController = player->AddComponent<PlayerController>();
    playerController->coinCollected.Add(&playerCoinEventItem);

    // Camera
    auto cameraController = GameObject::Create();
    cameraController->AddComponent<CameraController>(playerController);

    // Text
    auto scoreGameObject = GameObject::Create(
        Transform(
            glm::vec2(5.0f, 5.0f)
        )
    );
    scoreText = scoreGameObject->AddComponent<SimpleText2DLateRenderComponent>(fontSizePair);

    auto playTimeGameObject = GameObject::Create(
        Transform(
            glm::vec2(200.0f, 5.0f)
        )
    );
    playTimeText = playTimeGameObject->AddComponent<SimpleText2DLateRenderComponent>(fontSizePair);

#if L2DE_DEBUG
    auto fpsShower = FpsShower::CreateFpsShowerObject(
        Transform(
            glm::vec2(5.0f, RenderManager::GetInstance().GetResolution().GetHeight() - 30)
        ),
        fontSizePair,
        99);
#endif

    StartPlay();
}

void GameController::Destroy()
{
    playerController->coinCollected.Remove(&playerCoinEventItem);
    UpdaterComponent::Destroy();
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
    currentPlayTime = PLAY_TIME;
    RefreshScoreText();
    RefreshPlayTimeText();

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

    if (gameStatus == GameStatus::Play)
    {
        // If the player collected all coins
        if (playerController->coinNumber == coins.size())
            EndPlay();

        currentPlayTime -= Game::GetDeltaTime();
        if (currentPlayTime <= 0.0f)
        {
            currentPlayTime = 0;
            EndPlay();
        }
        RefreshPlayTimeText();
    }
}

void GameController::RefreshScoreText()
{
    scoreText->data.text = "Coin: " + std::to_string(playerController->coinNumber);
}

void GameController::RefreshPlayTimeText()
{
    std::string time = std::to_string(currentPlayTime);
    playTimeText->data.text = "Time: " + time.substr(0, time.find(".") + 3);
}

void GameController::Inform()
{
    RefreshScoreText();
}