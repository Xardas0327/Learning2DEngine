#include "GameController.h"

#include <Learning2DEngine/DebugTool/DebugMacro.h>
#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/Object/FpsShower.h>
#include <Learning2DEngine/UI/TextBoxComponent.h>
#if L2DE_DEBUG
#include <Learning2DEngine/DebugTool/DebugPosition.h>
#endif

#include "PlatformController.h"
#include "BushController.h"
#include "CameraController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Object;
using namespace Learning2DEngine::UI;
using namespace Learning2DEngine::DebugTool;
using namespace irrklang;

GameController::GameController(GameObject* gameObject)
    : UpdaterComponent(gameObject), Component(gameObject),
    coins(), movingPlatforms(), playerController(nullptr), gameStatus(GameStatus::Menu),
    fontSizePair("Assets/Fonts/PixelOperator8.ttf", 24), playerCoinEventItem(this), currentPlayTime(0),
    scoreText(nullptr), playTimeText(nullptr), descriptionText(nullptr), startText(nullptr), winText(nullptr),
    loseText(nullptr), endText(nullptr)
    , soundEngine(nullptr)
{

}

void GameController::Init()
{
    UpdaterComponent::Init();

    auto& gameObjectManager = GameObjectManager::GetInstance();
    gameObjectManager.Reserve(100);

#if L2DE_DEBUG
    DebugPosition::Init(FontSizePair("Assets/Fonts/PixelOperator8.ttf", 16));
#endif

    InitEnvironment();
    InitTexts();

    // Sounds
    soundEngine = createIrrKlangDevice();

    // I have to load a sound, because it gets stuck a bit on the first sound.
    ISound* sound = soundEngine->play2D("Assets/Sounds/jump.wav", false, false, true);
    if (sound) sound->stop();

    //Player
    auto player = gameObjectManager.CreateGameObject(Transform(PLAYER_START_POSITION));
    playerController = player->AddComponent<PlayerController>(
        soundEngine
    );
    playerController->coinCollected.Add(&playerCoinEventItem);
    playerController->rigidbody->isFrozen = true;

    // Camera
    auto cameraController = gameObjectManager.CreateGameObject();
    cameraController->AddComponent<CameraController>(playerController);

    ShowMenu();
}

void GameController::Destroy()
{
    playerController->coinCollected.Remove(&playerCoinEventItem);
    soundEngine->drop();
    UpdaterComponent::Destroy();
}

void GameController::InitEnvironment()
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
    movingPlatforms.reserve(2);
    movingPlatforms.push_back(MovingPlatformController::Create(glm::vec2(500.0f, 150.0f), glm::vec2(500.0f, 500.0f), "LargePlatform"));
    movingPlatforms.push_back(MovingPlatformController::Create(glm::vec2(950.0f, 450.0f), glm::vec2(1150.0f, 450.0f), "LargePlatform"));

    //Bush
    BushController::Create(glm::vec2(0.0f, 600.0f), "Bush2", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(50.0f, 600.0f), "Bush1", BUSH_SIZE, 1);
    BushController::Create(glm::vec2(200.0f, 600.0f), "Bush2", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(250.0f, 600.0f), "Bush1", BUSH_SIZE, 1);
    BushController::Create(glm::vec2(100.0f, 600.0f), "Bush2", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(150.0f, 600.0f), "Bush1", BUSH_SIZE, 1);
    BushController::Create(glm::vec2(200.0f, 600.0f), "Bush2", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(250.0f, 600.0f), "Bush1", BUSH_SIZE, 1);

    BushController::Create(glm::vec2(1100.0f, 600.0f), "Bush2", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(1150.0f, 600.0f), "Bush1", BUSH_SIZE, 1);
    BushController::Create(glm::vec2(1200.0f, 600.0f), "Bush2", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(1250.0f, 600.0f), "Bush1", BUSH_SIZE, 1);
    BushController::Create(glm::vec2(1300.0f, 600.0f), "Bush2", BUSH_SIZE, -1);
    BushController::Create(glm::vec2(1350.0f, 600.0f), "Bush1", BUSH_SIZE, 1);

    BushController::Create(glm::vec2(100.0f, 100.0f), "Bush4", BUSH_SIZE, 1);
    BushController::Create(glm::vec2(300.0f, 450.0f), "Bush3", BUSH_SIZE, -1);
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

void GameController::InitTexts()
{
    auto resolution = RenderManager::GetInstance().GetResolution();
    auto& gameObjectManager = GameObjectManager::GetInstance();

    //Score
    auto scoreGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(5.0f, 5.0f)
        )
    );
    scoreText = scoreGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, fontSizePair);

    //Play time
    auto playTimeGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(200.0f, 5.0f)
        )
    );
    playTimeText = playTimeGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, fontSizePair);

    //Description
    auto descriptionGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(200.0f, 300.0f)
        )
    );
    descriptionText = descriptionGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "Collect the coins in time.\nYou can move with A and D and jump with SPACE."
    );
    descriptionText->data.SetLineSpacing(25.0f);

    auto descriptionBox = descriptionGameObject->AddComponent<TextBoxComponent>(
        *descriptionText,
        TextBoxMode::SIMPLE,
        -1,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.7f)
    );
    descriptionBox->SetPadding(20.0f);

    // Start text
    auto startGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(resolution.GetWidth() / 2 - 150, 400.0f),
            glm::vec2(0.75f, 0.75f)
        )
    );
    startText = startGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "Press ENTER for start."
    );

    auto startBox = startGameObject->AddComponent<TextBoxComponent>(
        *startText,
        TextBoxMode::SIMPLE,
        -1,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.7f)
    );
    startBox->SetPadding(10.0f);

    // Win text
    auto winGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(resolution.GetWidth() / 2 - 150, 300.0f),
            glm::vec2(2.0f, 2.0f)
        )
    );
    winText = winGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "YOU WIN!",
        0,
        glm::vec4(0.08f, 0.43f, 0.185f, 1.0f)
    );

    auto winBox = winGameObject->AddComponent<TextBoxComponent>(
        *winText,
        TextBoxMode::SIMPLE,
        -1,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.7f)
    );
	winBox->paddingLeftRight = 65.0f;
	winBox->paddingTopBottom = 25.0f;

    // Lose text
    auto loseGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(resolution.GetWidth() / 2 - 175, 300.0f),
            glm::vec2(2.0f, 2.0f)
        )
    );
    loseText = loseGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "YOU LOSE!",
        0,
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );

    auto loseBox = loseGameObject->AddComponent<TextBoxComponent>(
        *loseText,
        TextBoxMode::SIMPLE,
        -1,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.7f)
    );
    loseBox->paddingLeftRight = 65.0f;
    loseBox->paddingTopBottom = 25.0f;

    // End text
    auto endGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(resolution.GetWidth() / 2 - 203, 380.0f),
            glm::vec2(0.75f, 0.75f)
        )
    );
    endText = endGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "Press ENTER for home screen."
    );

    auto endBox = endGameObject->AddComponent<TextBoxComponent>(
        *endText,
        TextBoxMode::SIMPLE,
        -1,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.7f)
    );
	endBox->SetPadding(10.0f);

#if L2DE_DEBUG
    FpsShower::CreateFpsShowerObject(
        Transform(
            glm::vec2(5.0f, resolution.GetHeight() - 30)
        ),
        fontSizePair,
        99);
#endif
}

void GameController::ShowMenu()
{
    descriptionText->isActive = true;
    startText->isActive = true;

    winText->isActive = false;
    loseText->isActive = false;
    endText->isActive = false;
    gameStatus = GameStatus::Menu;
}

void GameController::StartPlay()
{
    for (auto& coin : coins)
    {
        coin->gameObject->isActive = true;
    }
    for (auto& platform : movingPlatforms)
    {
        platform->Reset();
    }

    playerController->gameObject->transform.SetPosition(PLAYER_START_POSITION);
    playerController->rigidbody->isFrozen = false;
    playerController->rigidbody->velocity = glm::vec2(0.0f);
    playerController->coinNumber = 0;
    currentPlayTime = PLAY_TIME;

    RefreshScoreText();
    RefreshPlayTimeText();
    descriptionText->isActive = false;
    startText->isActive = false;

    gameStatus = GameStatus::Play;
}

void GameController::EndPlay()
{
    playerController->rigidbody->isFrozen = true;
    endText->isActive = true;

    if (playerController->coinNumber == coins.size())
        winText->isActive = true;
    else
        loseText->isActive = true;

    gameStatus = GameStatus::Ended;
}

void GameController::Update()
{
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_ESCAPE) == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    switch (gameStatus)
    {
    case GameStatus::Menu:
        if (Game::GetKeyboardButtonStatus(GLFW_KEY_ENTER) == InputStatus::KEY_DOWN)
        {
            StartPlay();
        }
        break;
    case GameStatus::Play:
        // If the player collected all coins
        if (playerController->coinNumber == coins.size())
            EndPlay();

        currentPlayTime -= Time::GetDeltaTime();
        if (currentPlayTime <= 0.0f)
        {
            currentPlayTime = 0;
            EndPlay();
        }
        RefreshPlayTimeText();
        break;
    case GameStatus::Ended:
        if (Game::GetKeyboardButtonStatus(GLFW_KEY_ENTER) == InputStatus::KEY_DOWN)
        {
            ShowMenu();
        }
        break;
    default:
        break;
    }
}

void GameController::RefreshScoreText()
{
    scoreText->data.SetText("Coin: " + std::to_string(playerController->coinNumber));
}

void GameController::RefreshPlayTimeText()
{
    std::string time = std::to_string(currentPlayTime);
    playTimeText->data.SetText("Time: " + time.substr(0, time.find(".") + 3));
}

void GameController::Inform()
{
    RefreshScoreText();
}