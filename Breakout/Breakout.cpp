#include "Breakout.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Random.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/UI/Text2DRenderer.h>
#include <Learning2DEngine/Physics/BoxCollider.h>
#include <Learning2DEngine/Physics/Collision.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;
using namespace Learning2DEngine::Physics;
using namespace irrklang;

Breakout::Breakout() :
    state(GameState::GAME_ACTIVE), powerUps(), levels(), selectedLevel(0), lives(3),
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
    state = GameState::GAME_MENU;
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
    Text2DRenderer::GetInstance().Load(fontSizePair);

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
    Game::cameraProjection = glm::ortho(
        0.0f,
        static_cast<float>(resolution.GetWidth()),
        static_cast<float>(resolution.GetHeight()),
        0.0f,
        -1.0f,
        1.0f
    );

    // Background
    auto background = new GameObject();
    backgroundController = background->AddComponent<BackgroundController, const std::string&, const Resolution&>("background", resolution);

    // Levels
    GameLevel one("Assets/Levels/one.lvl");
    one.Load();
    GameLevel two("Assets/Levels/two.lvl");
    two.Load();
    GameLevel three("Assets/Levels/three.lvl");
    three.Load();
    GameLevel four("Assets/Levels/four.lvl");
    four.Load();

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
            selectedLevel = (selectedLevel + 1) % levels.size();
        }
        if (Game::inputKeys[GLFW_KEY_S] == InputStatus::KEY_DOWN)
        {
            if (selectedLevel > 0)
                --selectedLevel;
            else
                selectedLevel = levels.size() -1;
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
                RenderManager::GetInstance().GetResolution().GetWidth() - playerController->gameObject->transform.scale.x)
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
    if (ballController->gameObject->transform.position.y >= RenderManager::GetInstance().GetResolution().GetHeight())
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

    postProcessData->RefreshShader(glfwGetTime());
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
    levels[selectedLevel].Load();
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

bool CheckCollision(const BoxCollider& box1, const BoxCollider& box2)
{
    return Collision::IsCollisoned(box1, box2).isCollisoned;
}

Direction VectorDirection(glm::vec2 target)
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

CollisionResult CheckCollision(const CircleCollider& ball, const BoxCollider& box) // AABB - Circle collision
{
    auto data = Collision::IsCollisoned(ball, box);
    if (data.isCollisoned)
    {
        glm::vec2 difference = data.edge2 - ball.GetCenter();
        return std::make_tuple(true, VectorDirection(difference), difference);
    }
    else
        return std::make_tuple(false, Direction::UP, glm::vec2(0.0f, 0.0f));
}

void Breakout::DoCollisions()
{
    for (BrickController* box : levels[selectedLevel].bricks)
    {
        if (box->gameObject->isActive)
        {
            CollisionResult collision = CheckCollision(*ballController->collider, *box->collider);
            if (std::get<0>(collision))
            {
                if (!box->isSolid)
                {
                    box->gameObject->isActive = false;
                    SpawnPowerUps(box->gameObject->transform.position);
                    soundEngine->play2D("Assets/Sounds/bleep.mp3", false);
                }
                else
                {   // Enable shake effect
                    shakeTime = 0.05f;
                    postProcessData->shake = true;
                    soundEngine->play2D("Assets/Sounds/solid.wav", false);
                }
                // collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (!(ballController->passThrough && !box->isSolid)) // don't do collision resolution on non-solid bricks if pass-through is activated
                {
                    if (dir == Direction::LEFT || dir == Direction::RIGHT) // horizontal collision
                    {
                        ballController->rigidbody->velocity.x = -ballController->rigidbody->velocity.x; // reverse horizontal velocity
                        // relocate
                        float penetration = ballController->radius - std::abs(diff_vector.x);
                        if (dir == Direction::LEFT)
                            ballController->gameObject->transform.position.x += penetration; // move ball to right
                        else
                            ballController->gameObject->transform.position.x -= penetration; // move ball to left;
                    }
                    else // vertical collision
                    {
                        ballController->rigidbody->velocity.y = -ballController->rigidbody->velocity.y; // reverse vertical velocity
                        // relocate
                        float penetration = ballController->radius - std::abs(diff_vector.y);
                        if (dir == Direction::UP)
                            ballController->gameObject->transform.position.y -= penetration; // move ball bback up
                        else
                            ballController->gameObject->transform.position.y += penetration; // move ball back down
                    }
                }
            }
        }
    }

    // also check collisions on PowerUps and if so, activate them
    for (PowerUpController* powerUp :powerUps)
    {
        if (powerUp->gameObject->isActive)
        {
            // first check if powerup passed bottom edge, if so: keep as inactive and destroy
            if (powerUp->gameObject->transform.position.y >= RenderManager::GetInstance().GetResolution().GetHeight())
                powerUp->gameObject->isActive = false;
            
            if (CheckCollision(*playerController->collider, *powerUp->collider))
            {	// collided with player, now activate powerup
                ActivatePowerUp(*powerUp);
                powerUp->gameObject->isActive = false;
                powerUp->activated = true;
                soundEngine->play2D("Assets/Sounds/powerup.wav", false);
            }
        }
    }
    // check collisions for player pad (unless stuck)
    CollisionResult result = CheckCollision(*ballController->collider, *playerController->collider);
    if (!ballController->stuck && std::get<0>(result))
    {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = playerController->gameObject->transform.position.x + playerController->gameObject->transform.scale.x / 2.0f;
        float distance = (ballController->gameObject->transform.position.x + ballController->radius) - centerBoard;
        float percentage = distance / (playerController->gameObject->transform.scale.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = ballController->rigidbody->velocity;
        ballController->rigidbody->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        ballController->rigidbody->velocity = glm::normalize(ballController->rigidbody->velocity) * glm::length(oldVelocity);
        // fix sticky paddle
        ballController->rigidbody->velocity.y = -1.0f * abs(ballController->rigidbody->velocity.y);
        ballController->stuck = ballController->sticky;

        soundEngine->play2D("Assets/Sounds/bleep.wav", false);
    }
}