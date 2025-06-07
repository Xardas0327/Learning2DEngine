#include "GameController.h"

#include <string>

#include <Learning2DEngine/DebugTool/DebugMacro.h>
#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/Random.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/Object/FpsShower.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::UI;
using namespace Learning2DEngine::Object;
#if USE_IRRKLANG_SOUND_ENGINE
using namespace irrklang;
#endif

GameController::GameController(GameObject* gameObject, const FontSizePair& fontSizePair, PostProcessData* postProcessData)
	: UpdaterComponent(gameObject), LateUpdaterComponent(gameObject), Component(gameObject),
    fontSizePair(fontSizePair), postProcessData(postProcessData),
    state(GameState::GAME_MENU), powerUps(), levels(), selectedLevel(0), lifes(3),
    backgroundController(nullptr), playerController(nullptr), ballController(nullptr),
    shakeTime(0.0f), lifeText(nullptr), startText(nullptr), levelSelectorText(nullptr), winText(nullptr), retryText(nullptr),
    powerUpActivationEventItem(this), ballHitPlayerEventItem(this), ballHitBrickEventItem(this)
#if USE_IRRKLANG_SOUND_ENGINE
    , soundEngine(nullptr)
#endif
{

}

void GameController::Init()
{
    UpdaterComponent::Init();
    LateUpdaterComponent::Init();

    const Resolution resolution = RenderManager::GetInstance().GetResolution();
    const int middleHeight = resolution.GetHeight() / 2;

    auto& gameObjectManager = GameObjectManager::GetInstance();
    //Maybe it is not enough, but it will reduce the useless reallocation.
    gameObjectManager.Reserve(200);

    // Background
    auto background = gameObjectManager.CreateGameObject();
    backgroundController = background->AddComponent<BackgroundController>("background", resolution);

    // Levels
    levels.reserve(4);
    levels.emplace_back("Assets/Levels/one.lvl");
    levels.emplace_back("Assets/Levels/two.lvl");
    levels.emplace_back("Assets/Levels/three.lvl");
    levels.emplace_back("Assets/Levels/four.lvl");
    levels[0].Load(true);
    levels[1].Load(false);
    levels[2].Load(false);
    levels[3].Load(false);
    selectedLevel = 0;

    // Player
    auto player = gameObjectManager.CreateGameObject();
    playerController = player->AddComponent<PlayerController>("paddle");

    // Ball
    auto ball = gameObjectManager.CreateGameObject();
    ballController = ball->AddComponent<BallController>(
            playerController,
            "face",
            "particle",
            ballHitPlayerEventItem,
            ballHitBrickEventItem
        );

    // Text
    auto liveGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(5.0f, 5.0f)
        )
    );
    lifeText = liveGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "Lifes: " + std::to_string(lifes)
    );

    auto startGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(250.0f, static_cast<float>(middleHeight))
        )
    );
    startText = startGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "Press ENTER to start"
    );

    auto levelSelectorGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(245.0f, static_cast<float>(middleHeight) + 20.0f),
            glm::vec2(0.75f, 0.75f)
        )
    );
    levelSelectorText = levelSelectorGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "Press W or S to select level"
    );

    auto winGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(320.0f, static_cast<float>(middleHeight) - 20.0f)
        ),
        false
    );
    winText = winGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "You WON!!!",
        0,
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
    );

    auto retryGameObject = gameObjectManager.CreateGameObject(
        Transform(
            glm::vec2(130.0f, static_cast<float>(middleHeight))
        ),
        false
    );
    retryText = retryGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "Press ENTER to retry or ESC to quit",
        0,
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)
    );

#if L2DE_DEBUG
    FpsShower::CreateFpsShowerObject(
        Transform(
            glm::vec2(5.0f, resolution.GetHeight() - 30)
        ),
        fontSizePair,
        99);
#endif

#if USE_IRRKLANG_SOUND_ENGINE
    // Sounds
    soundEngine = createIrrKlangDevice();
    soundEngine->play2D("Assets/Sounds/breakout.mp3", true);
#endif
}

void GameController::Destroy()
{
    UpdaterComponent::Destroy();
    LateUpdaterComponent::Destroy();
#if USE_IRRKLANG_SOUND_ENGINE
    soundEngine->drop();
#endif
}

void GameController::Update()
{
    ProcessInput();
    UpdatePowerUps();
    ShakeScreen(); 
}

void GameController::LateUpdate()
{
    postProcessData->RefreshShader(static_cast<float>(glfwGetTime()));
    IsLifeLost();
    IsLevelCompleted();
}

void GameController::ProcessInput()
{
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_ESCAPE) == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    switch (state)
    {
    case GameState::GAME_MENU:
        if (Game::GetKeyboardButtonStatus(GLFW_KEY_ENTER) == InputStatus::KEY_DOWN)
        {
            state = GameState::GAME_ACTIVE;
            startText->gameObject->isActive = false;
            levelSelectorText->gameObject->isActive = false;
            winText->gameObject->isActive = false;
            retryText->gameObject->isActive = false;
        }

        if (Game::GetKeyboardButtonStatus(GLFW_KEY_W) == InputStatus::KEY_DOWN)
        {
            size_t oldLevel = selectedLevel;
            selectedLevel = (selectedLevel + 1) % levels.size();

            levels[oldLevel].SetBricksActive(false);
            levels[selectedLevel].SetBricksActive(true);
        }
        if (Game::GetKeyboardButtonStatus(GLFW_KEY_S) == InputStatus::KEY_DOWN)
        {
            size_t oldLevel = selectedLevel;
            if (selectedLevel > 0)
                --selectedLevel;
            else
                selectedLevel = levels.size() - 1;

            levels[oldLevel].SetBricksActive(false);
            levels[selectedLevel].SetBricksActive(true);
        }
        break;
    case GameState::GAME_WIN:
        if (Game::GetKeyboardButtonStatus(GLFW_KEY_ENTER) == InputStatus::KEY_DOWN)
        {
            postProcessData->chaos = false;
            state = GameState::GAME_MENU;
            startText->gameObject->isActive = true;
            levelSelectorText->gameObject->isActive = true;
            winText->gameObject->isActive = false;
            retryText->gameObject->isActive = false;
        }
        break;
    case GameState::GAME_ACTIVE:
        float velocity = PLAYER_VELOCITY * Time::GetDeltaTime();
        if (Game::GetKeyboardButtonStatus(GLFW_KEY_A))
        {
            if (playerController->gameObject->transform.GetPosition().x >= 0.0f)
            {
                playerController->gameObject->transform.AddPosition(glm::vec2(-velocity, 0.0f));
                if (ballController->IsStuck())
                    ballController->gameObject->transform.AddPosition(glm::vec2(-velocity, 0.0f));
            }
        }
        if (Game::GetKeyboardButtonStatus(GLFW_KEY_D))
        {
            if (playerController->gameObject->transform.GetPosition().x <=
                Game::mainCamera.GetResolution().GetWidth() - playerController->gameObject->transform.GetScale().x)
            {
                playerController->gameObject->transform.AddPosition(glm::vec2(velocity, 0.0f));
                if (ballController->IsStuck())
                    ballController->gameObject->transform.AddPosition(glm::vec2(velocity, 0.0f));
            }
        }
        if (Game::GetKeyboardButtonStatus(GLFW_KEY_SPACE))
            ballController->SetStuck(false);
        break;
    }
}

void GameController::ShakeScreen()
{
    if (shakeTime > 0.0f)
    {
        shakeTime -= Time::GetDeltaTime();
        if (shakeTime <= 0.0f)
            postProcessData->shake = false;
    }
}

void GameController::IsLifeLost()
{
    if (ballController->gameObject->transform.GetPosition().y >= Game::mainCamera.GetResolution().GetHeight())
    {
        --lifes;
        if (lifes == 0)
        {
            ResetLevel();
            state = GameState::GAME_MENU;
            startText->gameObject->isActive = true;
            levelSelectorText->gameObject->isActive = true;
            winText->gameObject->isActive = false;
            retryText->gameObject->isActive = false;
        }
        else
        {
            RefreshLife();
        }
        ResetPlayer();
        ClearPowerUps();
    }
}

void GameController::IsLevelCompleted()
{
    if (state == GameState::GAME_ACTIVE && levels[selectedLevel].IsCompleted())
    {
        ResetLevel();
        ResetPlayer();
        ClearPowerUps();
        postProcessData->chaos = true;
        state = GameState::GAME_WIN;
        startText->gameObject->isActive = false;
        levelSelectorText->gameObject->isActive = false;
        winText->gameObject->isActive = true;
        retryText->gameObject->isActive = true;
    }
}

void GameController::ResetLevel()
{
    levels[selectedLevel].SetBricksActive(true);
    lifes = 3;
    RefreshLife();
}

void GameController::ResetPlayer()
{
    playerController->Reset();
    ballController->Reset();

    postProcessData->chaos = false;
    postProcessData->confuse = false;
}

void GameController::RefreshLife()
{
    lifeText->data.SetText("Lifes: " + std::to_string(lifes));
}

void GameController::SpawnPowerUps(glm::vec2 position)
{
    int number = Random::GetNumber(0, 50);
    if (number == 0)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpSpeed, position, powerUpActivationEventItem)
        );
    }
    else if (number == 1)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpSticky, position, powerUpActivationEventItem)
        );
    }
    else if (number == 2)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpPassThrough, position, powerUpActivationEventItem)
        );
    }
    else if (number == 3)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpPadSizeIncrease, position, powerUpActivationEventItem)
        );
    }
    else if (number > 3 && number < 8)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpConfuse, position, powerUpActivationEventItem)
        );
    }
    else if (number > 7 && number < 12)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpChaos, position, powerUpActivationEventItem)
        );
    }
}

bool GameController::IsPowerUpActive(const PowerUpType& type)
{
    for (const PowerUpController* powerUp : powerUps)
    {
        if (powerUp->activated && powerUp->powerUpObject.type == type)
            return true;
    }
    return false;
}

void GameController::UpdatePowerUps()
{
    for (PowerUpController* powerUp : powerUps)
    {
        if (powerUp->gameObject->transform.GetPosition().y >= Game::mainCamera.GetResolution().GetHeight())
            powerUp->gameObject->isActive = false;

        if (powerUp->activated)
        {
            powerUp->actualDuration -= Time::GetDeltaTime();

            if (powerUp->actualDuration <= 0.0f)
            {
                powerUp->activated = false;
                switch (powerUp->powerUpObject.type)
                {
                case PowerUpType::STICKY:
                    if (!IsPowerUpActive(PowerUpType::STICKY))
                    {
                        ballController->sticky = false;
                        playerController->renderer->data.color = glm::vec4(1.0f);
                    }
                    break;
                case PowerUpType::PASS_THROUGH:
                    if (!IsPowerUpActive(PowerUpType::PASS_THROUGH))
                    {
                        ballController->passThrough = false;
                        ballController->renderer->data.color = glm::vec4(1.0f);
                    }
                    break;
                case PowerUpType::CONFUSE:
                    if (!IsPowerUpActive(PowerUpType::CONFUSE))
                    {
                        postProcessData->confuse = false;
                    }
                    break;
                case PowerUpType::CHAOS:
                    if (!IsPowerUpActive(PowerUpType::CHAOS))
                    {
                        postProcessData->chaos = false;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    auto& gameObjectManager = GameObjectManager::GetInstance();
    powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
        [&gameObjectManager](PowerUpController* powerUp)
        {
            bool isDeletable = !powerUp->gameObject->isActive && !powerUp->activated;
            if (isDeletable)
            {
                gameObjectManager.DestroyGameObject(powerUp);
            }

            return isDeletable;
        }
    ), powerUps.end());
}

void GameController::ClearPowerUps()
{
    auto& gameObjectManager = GameObjectManager::GetInstance();
    for (PowerUpController* powerUp : powerUps)
    {
        gameObjectManager.DestroyGameObject(powerUp);
    }
    powerUps.clear();
}

void GameController::ActivatePowerUp(PowerUpType powerUpType)
{
    switch (powerUpType)
    {
    case PowerUpType::SPEED:
        ballController->rigidbody->velocity *= 1.2;
        break;
    case PowerUpType::STICKY:
        ballController->sticky = true;
        playerController->renderer->data.color = glm::vec4(1.0f, 0.5f, 1.0f, 1.0f);
        break;
    case PowerUpType::PASS_THROUGH:
        ballController->passThrough = true;
        ballController->renderer->data.color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);
        break;
    case PowerUpType::PAD_SIZE_INCREASE:
        playerController->gameObject->transform.AddScale(glm::vec2(50.0f, 0.0f));
        playerController->collider->colliderSize = playerController->gameObject->transform.GetScale();
        break;
    case PowerUpType::CONFUSE:
        // It will be activate only if chaos wasn't already active
        if (!postProcessData->chaos)
            postProcessData->confuse = true;
        break;
    case PowerUpType::CHAOS:
        // It will be activate only if confuse wasn't already active
        if (!postProcessData->confuse)
            postProcessData->chaos = true;
        break;
    default:
        break;
    }

#if USE_IRRKLANG_SOUND_ENGINE
    soundEngine->play2D("Assets/Sounds/powerup.wav", false);
#endif
}

void GameController::BallHitPlayer()
{
#if USE_IRRKLANG_SOUND_ENGINE
    soundEngine->play2D("Assets/Sounds/bleep.wav", false);
#endif
}

void GameController::BallHitBrick(BrickController* brick)
{
    if (!brick->IsSolid())
    {
        brick->gameObject->isActive = false;
        SpawnPowerUps(brick->gameObject->transform.GetPosition());
#if USE_IRRKLANG_SOUND_ENGINE
        soundEngine->play2D("Assets/Sounds/bleep.mp3", false);
#endif
    }
    else
    {
        shakeTime = 0.05f;
        postProcessData->shake = true;
#if USE_IRRKLANG_SOUND_ENGINE
        soundEngine->play2D("Assets/Sounds/solid.wav", false);
#endif
    }
}