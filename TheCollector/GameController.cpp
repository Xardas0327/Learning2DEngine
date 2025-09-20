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

#include <Learning2DEngine/Editor/TiledMapLoader.h>
using namespace Learning2DEngine::Editor;

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
    coins(), movingPlatforms(), box(nullptr), playerController(nullptr), gameStatus(GameStatus::Menu),
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
    DebugPosition::Init(FontSizePair("Assets/Fonts/PixelOperator8.ttf", 16), glm::vec2(0.1f, 0.1f), 1.0f);
#endif

    Game::mainCamera.SetResolution(Resolution(200, 115));
    Game::mainCamera.SetPosition(glm::vec2(0.0f, 40.0f));

	TiledMapLoader::LoadFromFile("Assets/Maps/Map.tmx");

    InitDynamicEnvironment();
    InitTexts();

    // Sounds
    soundEngine = createIrrKlangDevice();

    // I have to load a sound, because it gets stuck a bit on the first sound.
    ISound* sound = soundEngine->play2D("Assets/Sounds/jump.wav", false, false, true);
    if (sound)
        sound->stop();

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

void GameController::InitDynamicEnvironment()
{
	//Box
    box = BoxController::Create(glm::vec2(70.0f, 90.0f));

    //Moving Platform
    movingPlatforms.reserve(2);
    movingPlatforms.push_back(
        MovingPlatformController::Create(glm::vec2(90.0f, 65.0f), glm::vec2(90.0f, 120.0f), "Platforms", LARGE_PLATFORM_TEXTCOOR)
    );
    movingPlatforms.push_back(
        MovingPlatformController::Create(glm::vec2(176.0f, 111.0f), glm::vec2(200.0f, 111.0f), "Platforms", LARGE_PLATFORM_TEXTCOOR)
    );

    //Coin
    coins.reserve(5);
    coins.push_back(CoinController::Create(glm::vec2(20.0f, 45.0f)));
    coins.push_back(CoinController::Create(glm::vec2(20.0f, 120.0f)));
    coins.push_back(CoinController::Create(glm::vec2(160.0f, 120.0f)));
    coins.push_back(CoinController::Create(glm::vec2(245.0f, 98.0f)));
    coins.push_back(CoinController::Create(glm::vec2(195.0f, 45.0f)));

}

void GameController::InitTexts()
{
    auto& gameObjectManager = GameObjectManager::GetInstance();

    //Score
    auto scoreGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.2f, 0.2f)
        )
    );
    scoreText = scoreGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, fontSizePair);

    //Play time
    auto playTimeGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(35.0f, 1.0f),
            glm::vec2(0.2f, 0.2f)
        )
    );
    playTimeText = playTimeGameObject->AddComponent<SimpleText2DRenderComponent>(RendererMode::LATERENDER, fontSizePair);

    //Description
    auto descriptionGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(50.0f, 50.0f),
            glm::vec2(0.15f, 0.15f)
        )
    );
    descriptionText = descriptionGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "Collect the coins in time.\nYou can move with A and D and jump with SPACE."
    );
    descriptionText->data.SetLineSpacing(2.0f);

    auto descriptionBox = descriptionGameObject->AddComponent<TextBoxComponent>(
        *descriptionText,
        TextBoxMode::SIMPLE,
        -1,
        glm::vec4(0.0f, 0.0f, 0.0f, 0.8f)
    );
    descriptionBox->SetPadding(5.0f);

    // Start text
    auto startGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(Game::mainCamera.GetResolution().GetWidth() / 2 - 10.0f, 70.0f),
            glm::vec2(0.1f, 0.1f)
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
        glm::vec4(0.0f, 0.0f, 0.0f, 0.8f)
    );
    startBox->SetPadding(2.0f);

    // Win text
    auto winGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(Game::mainCamera.GetResolution().GetWidth() / 2 - 10.0f, 50.0f),
            glm::vec2(0.15f, 0.15f)
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
        glm::vec4(0.0f, 0.0f, 0.0f, 0.8f)
    );
	winBox->paddingLeftRight = 15.0f;
	winBox->paddingTopBottom = 5.0f;

    // Lose text
    auto loseGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(Game::mainCamera.GetResolution().GetWidth() / 2 - 12.0f, 50.0f),
            glm::vec2(0.15f, 0.15f)
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
        glm::vec4(0.0f, 0.0f, 0.0f, 0.9f)
    );
    loseBox->paddingLeftRight = 15.0f;
    loseBox->paddingTopBottom = 5.0f;

    // End text
    auto endGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(Game::mainCamera.GetResolution().GetWidth() / 2 - 26.0f, 62.0f),
            glm::vec2(0.1f, 0.1f)
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
        glm::vec4(0.0f, 0.0f, 0.0f, 0.8f)
    );
	endBox->SetPadding(1.0f);

#if L2DE_DEBUG
    auto fps = FpsShower::CreateFpsShowerObject(
        Transform(
            glm::vec2(2.0f, Game::mainCamera.GetResolution().GetHeight() - 5),
            glm::vec2(0.15f, 0.15f)
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
    box->ResetPosition();
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