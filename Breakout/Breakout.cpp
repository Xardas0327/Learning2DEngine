#include "Breakout.h"

#include <string>

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Random.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/UI/Text2DRenderer.h>
#include <Learning2DEngine/Physics/Collision.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;
using namespace Learning2DEngine::Physics;
using namespace irrklang;

Breakout::Breakout() :
    state(GameState::GAME_MENU), powerUps(), levels(), selectedLevel(0), lives(3),
    backgroundController(nullptr), playerController(nullptr), ballController(nullptr),
    soundEngine(nullptr), fontSizePair("Assets/Fonts/OCRAEXT.TTF", 24), postProcessData(nullptr),
    shakeTime(0.0f), liveText(), startText(), levelSelectorText(), winText(), retryText()
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
    auto& textRenderer = Text2DRenderer::GetInstance();
    textRenderer.Init(RenderManager::GetInstance().GetResolution());
    textRenderer.Load(fontSizePair);

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
    ballController = ball->AddComponent<BallController, PlayerController*, const std::string&, const std::string&>(playerController, "face", "particle");

    // Text
    liveText = {
        fontSizePair,
        "Lives: " + std::to_string(lives),
        5.0f,
        5.0f
    };
    startText = {
        fontSizePair,
        "Press ENTER to start",
        250.0f,
        static_cast<float>(middleHeight)
    };
    levelSelectorText = {
        fontSizePair,
        "Press W or S to select level",
        245.0f,
        static_cast<float>(middleHeight) + 20.0f,
        0.75f
    };
    winText = {
        fontSizePair,
        "You WON!!!",
        320.0f,
        static_cast<float>(middleHeight) - 20.0f,
        1.0,
        glm::vec3(0.0f, 1.0f, 0.0f)
    };
    retryText = {
        fontSizePair,
        "Press ENTER to retry or ESC to quit",
        130.0f,
        static_cast<float>(middleHeight),
        1.0f,
        glm::vec3(1.0f, 1.0f, 0.0f)
    };

}

void Breakout::Terminate()
{
    GameObject::Destroy(backgroundController);
    GameObject::Destroy(playerController);
    GameObject::Destroy(ballController);

    delete postProcessData;
    soundEngine->drop();

    Text2DRenderer::GetInstance().Terminate();
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
        }
        break;
    case GameState::GAME_ACTIVE:
        float velocity = PLAYER_VELOCITY * Game::GetDeltaTime();
        if (Game::inputKeys[GLFW_KEY_A])
        {
            if (playerController->gameObject->transform.position.x >= 0.0f)
            {
                playerController->gameObject->transform.position.x -= velocity;
                if (ballController->stuck)
                    ballController->gameObject->transform.position.x -= velocity;
            }
        }
        if (Game::inputKeys[GLFW_KEY_D])
        {
            if (playerController->gameObject->transform.position.x <=
                Game::mainCamera.GetResolution().GetWidth() - playerController->gameObject->transform.scale.x)
            {
                playerController->gameObject->transform.position.x += velocity;
                if (ballController->stuck)
                    ballController->gameObject->transform.position.x += velocity;
            }
        }
        if (Game::inputKeys[GLFW_KEY_SPACE])
            ballController->stuck = false;
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
        }
        else
        {
            liveText.text = "Lives: " + std::to_string(lives);
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
    }
}

void Breakout::Update()
{
    ProcessInput();

    ballController->Move();
    ballController->particleSystem->Update();

    DoCollisions();
    UpdatePowerUps();
    ShakeScreen();
    
    IsLiveLost();
    IsLevelCompleted();

    postProcessData->RefreshShader(glfwGetTime());
}

void Breakout::Render()
{
    backgroundController->renderer->Draw();

    levels[selectedLevel].Draw();

    ballController->particleSystem->Draw();

    playerController->renderer->Draw();

    for (PowerUpController* powerUp : powerUps)
    {
        if (powerUp->gameObject->isActive)
            powerUp->renderer->Draw();
    }

    ballController->renderer->Draw();
}

void Breakout::LateRender()
{
    auto& textRenderer = Text2DRenderer::GetInstance();
    textRenderer.RenderText(liveText);

    if (state == GameState::GAME_MENU)
    {
        textRenderer.RenderText(startText);
        textRenderer.RenderText(levelSelectorText);
    }
    else if (state == GameState::GAME_WIN)
    {
        textRenderer.RenderText(winText);
        textRenderer.RenderText(retryText);
    }
}

void Breakout::ResetLevel()
{
    levels[selectedLevel].Load(true);
    lives = 3;
    liveText.text = "Lives: " + std::to_string(lives);
}

void Breakout::ResetPlayer()
{
    playerController->Reset();
    ballController->Reset();

    postProcessData->chaos = false;
    postProcessData->confuse = false;
}

void Breakout::ActivatePowerUp(PowerUpController& powerUp)
{
    switch (powerUp.powerUpObject.type)
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
        playerController->collider->size = playerController->gameObject->transform.scale;
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
}

void Breakout::SpawnPowerUps(glm::vec2 position)
{
    int number = Random::GetNumber(0, 50);
    if (number == 0)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpSpeed, position)
        );
    }
    else if (number == 1)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpSticky, position)
        );
    }
    else if (number == 2)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpPassThrough, position)
        );
    }
    else if (number == 3)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpPadSizeIncrease, position)
        );
    }
    else if (number > 3 && number < 8)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpConfuse, position)
        );
    }
    else if (number > 7 && number < 12)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpChaos, position)
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
        powerUp->rigidbody->Update();
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

bool Breakout::CheckCollision(const BoxCollider& box1, const BoxCollider& box2)
{
    return Collision::IsCollisoned(box1, box2).isCollisoned;
}

Direction Breakout::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

CollisionResult Breakout::CheckCollision(const CircleCollider& ball, const BoxCollider& box)
{
    auto data = Collision::IsCollisoned(ball, box);
    if (data.isCollisoned)
    {
        glm::vec2 difference = data.edge2 - ball.GetCenter();
        return { true, VectorDirection(difference), difference };
    }
    else
        return { false, Direction::UP, glm::vec2(0.0f, 0.0f) };
}

void Breakout::CheckBricksCollision()
{
    for (BrickController* box : levels[selectedLevel].bricks)
    {
        if (box->gameObject->isActive)
        {
            CollisionResult collision = CheckCollision(*ballController->collider, *box->collider);
            if (collision.isCollisoned)
            {
                if (!box->isSolid)
                {
                    box->gameObject->isActive = false;
                    SpawnPowerUps(box->gameObject->transform.position);
                    soundEngine->play2D("Assets/Sounds/bleep.mp3", false);
                }
                else
                {
                    shakeTime = 0.05f;
                    postProcessData->shake = true;
                    soundEngine->play2D("Assets/Sounds/solid.wav", false);
                }

                if (!(ballController->passThrough && !box->isSolid))
                {
                    if (collision.direction == Direction::LEFT || collision.direction == Direction::RIGHT)
                    {
                        ballController->rigidbody->velocity.x = -ballController->rigidbody->velocity.x;

                        float penetration = ballController->radius - std::abs(collision.differenceVector.x);
                        if (collision.direction == Direction::LEFT)
                            ballController->gameObject->transform.position.x += penetration;
                        else
                            ballController->gameObject->transform.position.x -= penetration;
                    }
                    else
                    {
                        ballController->rigidbody->velocity.y = -ballController->rigidbody->velocity.y;

                        float penetration = ballController->radius - std::abs(collision.differenceVector.y);
                        if (collision.direction == Direction::UP)
                            ballController->gameObject->transform.position.y -= penetration;
                        else
                            ballController->gameObject->transform.position.y += penetration;
                    }
                }
            }
        }
    }
}

void Breakout::CheckPowerUpCollision()
{
    for (PowerUpController* powerUp : powerUps)
    {
        if (powerUp->gameObject->isActive)
        {
            if (powerUp->gameObject->transform.position.y >= Game::mainCamera.GetResolution().GetHeight())
                powerUp->gameObject->isActive = false;

            if (CheckCollision(*playerController->collider, *powerUp->collider))
            {
                ActivatePowerUp(*powerUp);
                powerUp->gameObject->isActive = false;
                powerUp->activated = true;
                soundEngine->play2D("Assets/Sounds/powerup.wav", false);
            }
        }
    }
}

void Breakout::CheckBallPlayerCollision()
{
    CollisionResult result = CheckCollision(*ballController->collider, *playerController->collider);
    if (!ballController->stuck && result.isCollisoned)
    {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = playerController->gameObject->transform.position.x + playerController->gameObject->transform.scale.x / 2.0f;
        float distance = (ballController->gameObject->transform.position.x + ballController->radius) - centerBoard;
        float percentage = distance / (playerController->gameObject->transform.scale.x / 2.0f);

        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = ballController->rigidbody->velocity;
        ballController->rigidbody->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;

        // keep speed consistent
        ballController->rigidbody->velocity = glm::normalize(ballController->rigidbody->velocity) * glm::length(oldVelocity);

        // fix sticky paddle
        ballController->rigidbody->velocity.y = -1.0f * abs(ballController->rigidbody->velocity.y);
        ballController->stuck = ballController->sticky;

        soundEngine->play2D("Assets/Sounds/bleep.wav", false);
    }
}

void Breakout::DoCollisions()
{
    CheckBricksCollision();
    CheckPowerUpCollision();
    CheckBallPlayerCollision();
}