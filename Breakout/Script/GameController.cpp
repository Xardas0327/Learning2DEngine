#include "GameController.h"

#include <string>

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/Random.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::UI;
using namespace irrklang;

GameController::GameController(GameObject* gameObject, const FontSizePair& fontSizePair, PostProcessData* postProcessData)
	: UpdaterComponent(gameObject), BaseUpdaterComponent(gameObject), Component(gameObject),
    fontSizePair(fontSizePair), postProcessData(postProcessData), soundEngine(nullptr),
    state(GameState::GAME_MENU), powerUps(), levels(), selectedLevel(0), lifes(3),
    backgroundController(nullptr), playerController(nullptr), ballController(nullptr),
    shakeTime(0.0f), lifeText(nullptr), startText(nullptr), levelSelectorText(nullptr), winText(nullptr), retryText(nullptr),
    powerUpActivationEventItem(this), ballHitPlayerEventItem(this), ballHitBrickEventItem(this)
{

}

void GameController::Init()
{
    UpdaterComponent::Init();

    const Resolution resolution = RenderManager::GetInstance().GetResolution();
    const int middleHeight = resolution.GetHeight() / 2;

    // Background
    auto background = new GameObject();
    backgroundController = background->AddComponent<BackgroundController, const std::string&, const Resolution&>("background", resolution);

    // Levels
    GameLevel one("Assets/Levels/one.lvl");
    one.Load(true);
    GameLevel two("Assets/Levels/two.lvl");
    two.Load(false);
    GameLevel three("Assets/Levels/three.lvl");
    three.Load(false);
    GameLevel four("Assets/Levels/four.lvl");
    four.Load(false);

    levels.push_back(one);
    levels.push_back(two);
    levels.push_back(three);
    levels.push_back(four);
    selectedLevel = 0;

    // Player
    auto player = new GameObject();
    playerController = player->AddComponent<PlayerController, const std::string&>("paddle");

    // Ball
    auto ball = new GameObject();
    ballController = ball->AddComponent<
        BallController,
        PlayerController*,
        const std::string&,
        const std::string&,
        BallHitPlayerEventItem&,
        BallHitBrickEventItem&>(
            playerController,
            "face",
            "particle",
            ballHitPlayerEventItem,
            ballHitBrickEventItem
        );

    // Text
    auto liveGameObject = new GameObject(
        Transform(
            glm::vec2(5.0f, 5.0f)
        )
    );
    lifeText = liveGameObject->AddComponent<Text2DLateRenderer, const Resolution&, const FontSizePair&, std::string>(
        resolution,
        fontSizePair,
        "Lifes: " + std::to_string(lifes)
    );

    auto startGameObject = new GameObject(
        Transform(
            glm::vec2(250.0f, static_cast<float>(middleHeight))
        )
    );
    startText = startGameObject->AddComponent<Text2DLateRenderer, const Resolution&, const FontSizePair&, std::string>(
        resolution,
        fontSizePair,
        "Press ENTER to start"
    );

    auto levelSelectorGameObject = new GameObject(
        Transform(
            glm::vec2(245.0f, static_cast<float>(middleHeight) + 20.0f),
            glm::vec2(0.75f, 0.75f)
        )
    );
    levelSelectorText = levelSelectorGameObject->AddComponent<Text2DLateRenderer, const Resolution&, const FontSizePair&, std::string>(
        resolution,
        fontSizePair,
        "Press W or S to select level"
    );

    auto winGameObject = new GameObject(
        Transform(
            glm::vec2(320.0f, static_cast<float>(middleHeight) - 20.0f)
        ),
        false
    );
    winText = winGameObject->AddComponent<Text2DLateRenderer, const Resolution&, const FontSizePair&, std::string, int, glm::vec3>(
        resolution,
        fontSizePair,
        "You WON!!!",
        0,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    auto retryGameObject = new GameObject(
        Transform(
            glm::vec2(130.0f, static_cast<float>(middleHeight))
        ),
        false
    );
    retryText = retryGameObject->AddComponent<Text2DLateRenderer, const Resolution&, const FontSizePair&, std::string, int, glm::vec3>(
        resolution,
        fontSizePair,
        "Press ENTER to retry or ESC to quit",
        0,
        glm::vec3(1.0f, 1.0f, 0.0f)
    );

    // Sounds
    soundEngine = createIrrKlangDevice();
    soundEngine->play2D("Assets/Sounds/breakout.mp3", true);
}

void GameController::Destroy()
{
    UpdaterComponent::Destroy();

    GameObject::Destroy(backgroundController);
    GameObject::Destroy(playerController);
    GameObject::Destroy(ballController);

    GameObject::Destroy(lifeText);
    GameObject::Destroy(startText);
    GameObject::Destroy(levelSelectorText);
    GameObject::Destroy(winText);
    GameObject::Destroy(retryText);

    for (auto& level : levels)
        level.ClearBricks();

    soundEngine->drop();
}

void GameController::Update()
{
    //Update
    ProcessInput();
    UpdatePowerUps();
    ShakeScreen(); 

    //DoCollisions();

    //LateUpdate
    postProcessData->RefreshShader(glfwGetTime());
    IsLifeLost();
    IsLevelCompleted(); 
}

void GameController::ProcessInput()
{
    if (Game::GetInputStatus(GLFW_KEY_ESCAPE) == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    switch (state)
    {
    case GameState::GAME_MENU:
        if (Game::GetInputStatus(GLFW_KEY_ENTER) == InputStatus::KEY_DOWN)
        {
            state = GameState::GAME_ACTIVE;
            startText->gameObject->isActive = false;
            levelSelectorText->gameObject->isActive = false;
            winText->gameObject->isActive = false;
            retryText->gameObject->isActive = false;
        }

        if (Game::GetInputStatus(GLFW_KEY_W) == InputStatus::KEY_DOWN)
        {
            int oldLevel = selectedLevel;
            selectedLevel = (selectedLevel + 1) % levels.size();

            levels[oldLevel].SetBricksActive(false);
            levels[selectedLevel].SetBricksActive(true);
        }
        if (Game::GetInputStatus(GLFW_KEY_S) == InputStatus::KEY_DOWN)
        {
            int oldLevel = selectedLevel;
            if (selectedLevel > 0)
                --selectedLevel;
            else
                selectedLevel = levels.size() - 1;

            levels[oldLevel].SetBricksActive(false);
            levels[selectedLevel].SetBricksActive(true);
        }
        break;
    case GameState::GAME_WIN:
        if (Game::GetInputStatus(GLFW_KEY_ENTER) == InputStatus::KEY_DOWN)
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
        float velocity = PLAYER_VELOCITY * Game::GetDeltaTime();
        if (Game::GetInputStatus(GLFW_KEY_A))
        {
            if (playerController->gameObject->transform.position.x >= 0.0f)
            {
                playerController->gameObject->transform.position.x -= velocity;
                if (ballController->IsStuck())
                    ballController->gameObject->transform.position.x -= velocity;
            }
        }
        if (Game::GetInputStatus(GLFW_KEY_D))
        {
            if (playerController->gameObject->transform.position.x <=
                Game::mainCamera.GetResolution().GetWidth() - playerController->gameObject->transform.scale.x)
            {
                playerController->gameObject->transform.position.x += velocity;
                if (ballController->IsStuck())
                    ballController->gameObject->transform.position.x += velocity;
            }
        }
        if (Game::GetInputStatus(GLFW_KEY_SPACE))
            ballController->SetStuck(false);
        break;
    }
}

void GameController::ShakeScreen()
{
    if (shakeTime > 0.0f)
    {
        shakeTime -= Game::GetDeltaTime();
        if (shakeTime <= 0.0f)
            postProcessData->shake = false;
    }
}

void GameController::IsLifeLost()
{
    if (ballController->gameObject->transform.position.y >= Game::mainCamera.GetResolution().GetHeight())
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
            lifeText->text = "Lifes: " + std::to_string(lifes);
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
    levels[selectedLevel].Load(true);
    lifes = 3;
    lifeText->text = "Lifes: " + std::to_string(lifes);
}

void GameController::ResetPlayer()
{
    playerController->Reset();
    ballController->Reset();

    postProcessData->chaos = false;
    postProcessData->confuse = false;
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
        if (powerUp->gameObject->transform.position.y >= Game::mainCamera.GetResolution().GetHeight())
            powerUp->gameObject->isActive = false;

        if (powerUp->activated)
        {
            powerUp->actualDuration -= Game::GetDeltaTime();

            if (powerUp->actualDuration <= 0.0f)
            {
                powerUp->activated = false;
                switch (powerUp->powerUpObject.type)
                {
                case PowerUpType::STICKY:
                    if (!IsPowerUpActive(PowerUpType::STICKY))
                    {
                        ballController->sticky = false;
                        playerController->renderer->color = glm::vec3(1.0f);
                    }
                    break;
                case PowerUpType::PASS_THROUGH:
                    if (!IsPowerUpActive(PowerUpType::PASS_THROUGH))
                    {
                        ballController->passThrough = false;
                        ballController->renderer->color = glm::vec3(1.0f);
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

    powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
        [](PowerUpController* powerUp)
        {
            bool isDeletable = !powerUp->gameObject->isActive && !powerUp->activated;
            if (isDeletable)
            {
                GameObject::Destroy(powerUp);
            }

            return isDeletable;
        }
    ), powerUps.end());
}

void GameController::ClearPowerUps()
{
    for (PowerUpController* powerUp : powerUps)
    {
        GameObject::Destroy(powerUp);
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
        playerController->renderer->color = glm::vec3(1.0f, 0.5f, 1.0f);
        break;
    case PowerUpType::PASS_THROUGH:
        ballController->passThrough = true;
        ballController->renderer->color = glm::vec3(1.0f, 0.5f, 0.5f);
        break;
    case PowerUpType::PAD_SIZE_INCREASE:
        playerController->gameObject->transform.scale.x += 50;
        playerController->collider->colliderSize = playerController->gameObject->transform.scale;
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

    soundEngine->play2D("Assets/Sounds/powerup.wav", false);
}

void GameController::BallHitPlayer()
{
    soundEngine->play2D("Assets/Sounds/bleep.wav", false);
}

void GameController::BallHitBrick(BrickController* brick)
{
    if (!brick->IsSolid())
    {
        brick->gameObject->isActive = false;
        SpawnPowerUps(brick->gameObject->transform.position);
        soundEngine->play2D("Assets/Sounds/bleep.mp3", false);
    }
    else
    {
        shakeTime = 0.05f;
        postProcessData->shake = true;
        soundEngine->play2D("Assets/Sounds/solid.wav", false);
    }
}