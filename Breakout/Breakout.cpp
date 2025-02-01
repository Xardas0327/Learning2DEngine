#include "Breakout.h"

#include <string>

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Random.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/UI/TextCharacterSet.h>
#include <Learning2DEngine/Physics/CollisionChecker.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;
using namespace Learning2DEngine::Physics;
using namespace irrklang;

Breakout::Breakout() :
    state(GameState::GAME_MENU), powerUps(), levels(), selectedLevel(0), lives(3),
    backgroundController(nullptr), playerController(nullptr), ballController(nullptr),
    soundEngine(nullptr), fontSizePair("Assets/Fonts/OCRAEXT.TTF", 24), postProcessData(nullptr),
    shakeTime(0.0f), liveText(nullptr), startText(nullptr), levelSelectorText(nullptr), winText(nullptr), retryText(nullptr),
    powerUpActivationEventItem(this), ballHitPlayerEventItem(this), ballHitBrickEventItem(this)
{

}

Breakout::~Breakout()
{
}

void Breakout::Init()
{
    Game::Init();

    InitSystem();
    InitObjects();

    soundEngine->play2D("Assets/Sounds/breakout.mp3", true);
}

void Breakout::InitSystem()
{
    auto& resourceManager = ResourceManager::GetInstance();

    // Shaders
    resourceManager.LoadShaderFromFile(std::string("PostProcessing"), "Assets/Shaders/PostProcessing.vs", "Assets/Shaders/PostProcessing.fs");

    // Textures
    Texture2DSettings alphaSettings;
    alphaSettings.internalFormat = GL_RGBA;
    alphaSettings.imageFormat = GL_RGBA;
    Texture2DSettings nonAlphaSettings;

    resourceManager.LoadTextureFromFile("background", "Assets/Images/background.jpg", nonAlphaSettings);
    resourceManager.LoadTextureFromFile("face", "Assets/Images/awesomeface.png", alphaSettings);
    resourceManager.LoadTextureFromFile("block", "Assets/Images/block.png", nonAlphaSettings);
    resourceManager.LoadTextureFromFile("block_solid", "Assets/Images/block_solid.png", nonAlphaSettings);
    resourceManager.LoadTextureFromFile("paddle", "Assets/Images/paddle.png", alphaSettings);
    resourceManager.LoadTextureFromFile("particle", "Assets/Images/particle.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_speed", "Assets/Images/powerup_speed.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_sticky", "Assets/Images/powerup_sticky.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_increase", "Assets/Images/powerup_increase.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_confuse", "Assets/Images/powerup_confuse.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_chaos", "Assets/Images/powerup_chaos.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_passthrough", "Assets/Images/powerup_passthrough.png", alphaSettings);

    // Text
    TextCharacterSet::GetInstance().Load(fontSizePair);

    // MSAA
    ActivateMSAA(4);

    // PostProcessEffect
    postProcessData = new PostProcessData(resourceManager.GetShader("PostProcessing"));
    ActivatePostProcessEffect();
    UsePostProcessEffect(postProcessData->shader);

    // Sounds
    soundEngine = createIrrKlangDevice();
}

void Breakout::InitObjects()
{
    const Resolution resolution = RenderManager::GetInstance().GetResolution();
    const int middleHeight = resolution.GetHeight() / 2;

    // Camera
    mainCamera.SetResolution(resolution);

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
    liveText = liveGameObject->AddComponent<Text2DLateRenderer, const Resolution&, const FontSizePair&, std::string>(
        resolution,
        fontSizePair,
        "Lives: " + std::to_string(lives)
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

}

void Breakout::Terminate()
{
    GameObject::Destroy(backgroundController);
    GameObject::Destroy(playerController);
    GameObject::Destroy(ballController);

	GameObject::Destroy(liveText);
	GameObject::Destroy(startText);
	GameObject::Destroy(levelSelectorText);
	GameObject::Destroy(winText);
	GameObject::Destroy(retryText);

	for (auto& level : levels)
		level.ClearBricks();

    delete postProcessData;
    soundEngine->drop();

    TextCharacterSet::GetInstance().Clear();
    Game::Terminate();
}

void Breakout::ProcessInput()
{
    if (Game::inputKeys[GLFW_KEY_ESCAPE] == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    switch (state)
    {
    case GameState::GAME_MENU:
        if (Game::inputKeys[GLFW_KEY_ENTER] == InputStatus::KEY_DOWN)
        {
            state = GameState::GAME_ACTIVE;
			startText->gameObject->isActive = false;
			levelSelectorText->gameObject->isActive = false;
			winText->gameObject->isActive = false;
			retryText->gameObject->isActive = false;
        }

        if (Game::inputKeys[GLFW_KEY_W] == InputStatus::KEY_DOWN)
        {
			int oldLevel = selectedLevel;
            selectedLevel = (selectedLevel + 1) % levels.size();

            levels[oldLevel].SetBricksActive(false);
			levels[selectedLevel].SetBricksActive(true);
        }
        if (Game::inputKeys[GLFW_KEY_S] == InputStatus::KEY_DOWN)
        {
            int oldLevel = selectedLevel;
            if (selectedLevel > 0)
                --selectedLevel;
            else
                selectedLevel = levels.size() -1;

            levels[oldLevel].SetBricksActive(false);
            levels[selectedLevel].SetBricksActive(true);
        }
        break;
    case GameState::GAME_WIN:
        if (Game::inputKeys[GLFW_KEY_ENTER] == InputStatus::KEY_DOWN)
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
        if (Game::inputKeys[GLFW_KEY_A])
        {
            if (playerController->gameObject->transform.position.x >= 0.0f)
            {
                playerController->gameObject->transform.position.x -= velocity;
                if (ballController->IsStuck())
                    ballController->gameObject->transform.position.x -= velocity;
            }
        }
        if (Game::inputKeys[GLFW_KEY_D])
        {
            if (playerController->gameObject->transform.position.x <=
                Game::mainCamera.GetResolution().GetWidth() - playerController->gameObject->transform.scale.x)
            {
                playerController->gameObject->transform.position.x += velocity;
                if (ballController->IsStuck())
                    ballController->gameObject->transform.position.x += velocity;
            }
        }
        if (Game::inputKeys[GLFW_KEY_SPACE])
            ballController->SetStuck(false);
        break;
    }
}

void Breakout::ShakeScreen()
{
    if (shakeTime > 0.0f)
    {
        shakeTime -= Game::GetDeltaTime();
        if (shakeTime <= 0.0f)
            postProcessData->shake = false;
    }
}

void Breakout::IsLiveLost()
{
    if (ballController->gameObject->transform.position.y >= Game::mainCamera.GetResolution().GetHeight())
    {
        --lives;
        if (lives == 0)
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
			liveText->text = "Lives: " + std::to_string(lives);
        }
        ResetPlayer();
        ClearPowerUps();
    }
}

void Breakout::IsLevelCompleted()
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

void Breakout::Update()
{
    ProcessInput();

    //DoCollisions();
    UpdatePowerUps();
    ShakeScreen();
    
    IsLiveLost();
    IsLevelCompleted();

    postProcessData->RefreshShader(glfwGetTime());
}

void Breakout::ResetLevel()
{
    levels[selectedLevel].Load(true);
    lives = 3;
    liveText->text = "Lives: " + std::to_string(lives);
}

void Breakout::ResetPlayer()
{
    playerController->Reset();
    ballController->Reset();

    postProcessData->chaos = false;
    postProcessData->confuse = false;
}

void Breakout::ActivatePowerUp(PowerUpType powerUpType)
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

void Breakout::SpawnPowerUps(glm::vec2 position)
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

bool Breakout::IsPowerUpActive(const PowerUpType& type)
{
    for (const PowerUpController* powerUp : powerUps)
    {
        if (powerUp->activated && powerUp->powerUpObject.type == type)
            return true;
    }
    return false;
}

void Breakout::UpdatePowerUps()
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

void Breakout::ClearPowerUps()
{
    for (PowerUpController* powerUp : powerUps)
    {
        GameObject::Destroy(powerUp);
    }
    powerUps.clear();
}

void Breakout::BallHitPlayer()
{
    soundEngine->play2D("Assets/Sounds/bleep.wav", false);
}

void Breakout::BallHitBrick(BrickController* brick)
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