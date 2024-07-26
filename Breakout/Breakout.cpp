#include "Breakout.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <irrklang/irrKlang.h>
#include <string>

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Random.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/UI/Text2DRenderer.h>
#include <Learning2DEngine/ParticleSimulator/ParticleSystem.h>
#include <Learning2DEngine/Physics/BoxCollider.h>
#include <Learning2DEngine/Physics/Collision.h>

#include "BallController.h"
#include "PowerUpType.h"
#include "PowerUpObject.h"
#include "BallParticleSettings.h"
#include "PostProcessData.h"


using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;
using namespace Learning2DEngine::ParticleSimulator;
using namespace Learning2DEngine::Physics;
using namespace irrklang;

// Game-related State data
GameObject* Ball;
PostProcessData* postProcessData;
ISoundEngine* SoundEngine = createIrrKlangDevice();

float ShakeTime = 0.0f;

const FontSizePair fontSizePair("Assets/Fonts/OCRAEXT.TTF", 24);

Breakout::Breakout() :
    state(GAME_ACTIVE), powerUps(), levels(), selectedLevel(0), lives(3),
    backgroundController(nullptr), playerController(nullptr),
    liveText(), startText(), levelSelectorText(), winText(), retryText()
{

}

Breakout::~Breakout()
{
}

void Breakout::Init()
{
    Game::Init();

    auto& resourceManager = ResourceManager::GetInstance();
    const Resolution resolution = RenderManager::GetInstance().GetResolution();
    const int middleHeight = resolution.GetHeight() / 2;

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

    // Player
    auto player = new GameObject();
    playerController = player->AddComponent<PlayerController, const std::string&>("paddle");

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





    // Ball
    glm::vec2 ballPos = player->transform.position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new GameObject(
        Transform(
            ballPos,
            glm::vec2(BALL_RADIUS * 2.0f, BALL_RADIUS * 2.0f)
        )
    );
    Ball->AddComponent<SpriteRenderer, const Texture2D&>(
        resourceManager.GetTexture("face")
    );
    auto ballController = Ball->AddComponent<BallController, float>(BALL_RADIUS);

    ParticleSystemSettings ballParticleSystemSettings(
        true,
        BlendFuncFactor(GL_SRC_ALPHA, GL_ONE),
        0.001f,
        1,
        0.0f
    );
    auto ballParticleSystem = Ball->AddComponent<ParticleSystem, unsigned int, const Texture2D&, const ParticleSystemSettings&>(
        500,
        resourceManager.GetTexture("particle"),
        ballParticleSystemSettings,
        new BallParticleSettings(glm::vec2(BALL_RADIUS / 2.0f), glm::vec2(10.0f, 10.0f), 0.3f, 0.1f)
    );
    ballParticleSystem->Start();

    // Sounds
    SoundEngine->play2D("Assets/Sounds/breakout.mp3", true);

    // Text
    auto& textRenderer = Text2DRenderer::GetInstance();
    textRenderer.Load(fontSizePair);

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

    // State
    state = GAME_MENU;

    //MSAA
    ActivateMSAA(4);

    //PostProcessEffect
    postProcessData = new PostProcessData(resourceManager.GetShader("PostProcessing"));
    ActivatePostProcessEffect();
    UsePostProcessEffect(postProcessData->shader);
}

void Breakout::Terminate()
{
    GameObject::Destroy(backgroundController);
    GameObject::Destroy(playerController);
    delete Ball;
    delete postProcessData;
    SoundEngine->drop();
    Game::Terminate();
}

void Breakout::ProcessInput()
{
    if (Game::inputKeys[GLFW_KEY_ESCAPE] == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    if (state == GAME_MENU)
    {
        if (Game::inputKeys[GLFW_KEY_ENTER] == InputStatus::KEY_DOWN)
        {
            state = GAME_ACTIVE;
        }

        if (Game::inputKeys[GLFW_KEY_W] == InputStatus::KEY_DOWN)
        {
            selectedLevel = (selectedLevel + 1) % 4;
        }
        if (Game::inputKeys[GLFW_KEY_S] == InputStatus::KEY_DOWN)
        {
            if (selectedLevel > 0)
                --selectedLevel;
            else
                selectedLevel = 3;
        }
    }
    if (state == GAME_ACTIVE)
    {
        auto ballController = Ball->GetComponent<BallController>();
        float velocity = PLAYER_VELOCITY * Game::GetDeltaTime();
        // move playerboard
        if (Game::inputKeys[GLFW_KEY_A])
        {
            if (playerController->gameObject->transform.position.x >= 0.0f)
            {
                playerController->gameObject->transform.position.x -= velocity;
                if (ballController->stuck)
                    Ball->transform.position.x -= velocity;
            }
        }
        if (Game::inputKeys[GLFW_KEY_D])
        {
            if (playerController->gameObject->transform.position.x <= 
                RenderManager::GetInstance().GetResolution().GetWidth() - playerController->gameObject->transform.scale.x)
            {
                playerController->gameObject->transform.position.x += velocity;
                if (ballController->stuck)
                    Ball->transform.position.x += velocity;
            }
        }
        if (Game::inputKeys[GLFW_KEY_SPACE])
            ballController->stuck = false;
    }

    if (state == GAME_WIN)
    {
        if (Game::inputKeys[GLFW_KEY_ENTER] == InputStatus::KEY_DOWN)
        {
            postProcessData->chaos = false;
            state = GAME_MENU;
        }
    }
}

void Breakout::Update()
{
    ProcessInput();

    Ball->GetComponent<BallController>()->Move();
    DoCollisions();
    Ball->GetComponent<ParticleSystem>()->Update();
    UpdatePowerUps();

    if (ShakeTime > 0.0f)
    {
        ShakeTime -= Game::GetDeltaTime();
        if (ShakeTime <= 0.0f)
            postProcessData->shake = false;
    }

    // Lose live
    if (Ball->transform.position.y >= RenderManager::GetInstance().GetResolution().GetHeight())
    {
        --lives;
        // Game over
        if (lives == 0)
        {
            ResetLevel();
            state = GAME_MENU;
        }
        else
        {
            liveText.text = "Lives: " + std::to_string(lives);
        }
        ResetPlayer();
        ClearPowerUps();
    }

    // check win condition
    if (state == GAME_ACTIVE && levels[selectedLevel].IsCompleted())
    {
        ResetLevel();
        ResetPlayer();
        ClearPowerUps();
        postProcessData->chaos = true;
        state = GAME_WIN;
    }
}

void Breakout::Render()
{
    backgroundController->renderer->Draw();

    levels[selectedLevel].Draw();

    playerController->renderer->Draw();

    // Draw PowerUps
    for (PowerUpController* powerUp : powerUps)
    {
        if (powerUp->gameObject->isActive)
            powerUp->gameObject->GetComponent<SpriteRenderer>()->Draw();
    }
    // Draw Particles
    Ball->GetComponent<ParticleSystem>()->Draw();
    // Draw Player
    Ball->GetComponent<SpriteRenderer>()->Draw();
    // Refresh the PostProcess Shader
    postProcessData->RefreshShader(glfwGetTime());
}

void Breakout::LateRender()
{
    auto& textRenderer = Text2DRenderer::GetInstance();
    textRenderer.RenderText(liveText);

    if (state == GAME_MENU)
    {
        textRenderer.RenderText(startText);
        textRenderer.RenderText(levelSelectorText);
    }

    if (state == GAME_WIN)
    {
        textRenderer.RenderText(winText);
        textRenderer.RenderText(retryText);
    }
}

void Breakout::ActivatePowerUp(PowerUpController& powerUp)
{
    auto ballController = Ball->GetComponent<BallController>();
    auto ballRenderer = Ball->GetComponent<SpriteRenderer>();

    switch (powerUp.type)
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
        ballRenderer->color = glm::vec3(1.0f, 0.5f, 0.5f);
        break;
    case PowerUpType::PAD_SIZE_INCREASE:
        playerController->gameObject->transform.scale.x += 50;
        playerController->collider->size = playerController->gameObject->transform.scale;
        break;
    case PowerUpType::CONFUSE:
        // only activate if chaos wasn't already active
        if (!postProcessData->chaos)
            postProcessData->confuse = true;
        break;
    case PowerUpType::CHAOS:
        if (!postProcessData->confuse)
            postProcessData->chaos = true;
        break;
    default:
        break;
    }
}

// AABB - AABB collision
bool CheckCollision(const BoxCollider& box1, const BoxCollider& box2)
{
    return Collision::IsCollisoned(box1, box2).isCollisoned;
}

// calculates which direction a vector is facing (N,E,S or W)
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
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

void Breakout::DoCollisions()
{
    auto ballController = Ball->GetComponent<BallController>();
    for (BrickController* box : levels[selectedLevel].bricks)
    {
        if (box->gameObject->isActive)
        {
            CollisionResult collision = CheckCollision(*ballController->collider, *box->collider);
            if (std::get<0>(collision)) // if collision is true
            {
                // destroy block if not solid
                if (!box->isSolid)
                {
                    box->gameObject->isActive = false;
                    SpawnPowerUps(*box->gameObject);
                    SoundEngine->play2D("Assets/Sounds/bleep.mp3", false);
                }
                else
                {   // if block is solid, enable shake effect
                    ShakeTime = 0.05f;
                    postProcessData->shake = true;
                    SoundEngine->play2D("Assets/Sounds/solid.wav", false);
                }
                // collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (!(ballController->passThrough && !box->isSolid)) // don't do collision resolution on non-solid bricks if pass-through is activated
                {
                    if (dir == LEFT || dir == RIGHT) // horizontal collision
                    {
                        ballController->rigidbody->velocity.x = -ballController->rigidbody->velocity.x; // reverse horizontal velocity
                        // relocate
                        float penetration = ballController->radius - std::abs(diff_vector.x);
                        if (dir == LEFT)
                            Ball->transform.position.x += penetration; // move ball to right
                        else
                            Ball->transform.position.x -= penetration; // move ball to left;
                    }
                    else // vertical collision
                    {
                        ballController->rigidbody->velocity.y = -ballController->rigidbody->velocity.y; // reverse vertical velocity
                        // relocate
                        float penetration = ballController->radius - std::abs(diff_vector.y);
                        if (dir == UP)
                            Ball->transform.position.y -= penetration; // move ball bback up
                        else
                            Ball->transform.position.y += penetration; // move ball back down
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
                SoundEngine->play2D("Assets/Sounds/powerup.wav", false);
            }
        }
    }
    // check collisions for player pad (unless stuck)
    CollisionResult result = CheckCollision(*ballController->collider, *playerController->collider);
    if (!ballController->stuck && std::get<0>(result))
    {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = playerController->gameObject->transform.position.x + playerController->gameObject->transform.scale.x / 2.0f;
        float distance = (Ball->transform.position.x + ballController->radius) - centerBoard;
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

        SoundEngine->play2D("Assets/Sounds/bleep.wav", false);
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
    // reset player/ball stats
    playerController->Reset();
    auto ballController = Ball->GetComponent<BallController>();
    ballController->Reset(playerController->gameObject->transform.position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);

    // also disable all active powerups
    postProcessData->chaos = false;
    postProcessData->confuse = false;
    ballController->passThrough = false;
    ballController->sticky = false;

    auto ballRenderer = Ball->GetComponent<SpriteRenderer>();
    ballRenderer->color = glm::vec3(1.0f);
}

void Breakout::SpawnPowerUps(GameObject& block)
{
    int number = Random::GetNumber(0, 50);
    if (number == 0)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpSpeed, block.transform.position)
        );
    }
    else if (number == 1)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpSticky, block.transform.position)
        );
    }
    else if (number == 2)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpPassThrough, block.transform.position)
        );
    }
    else if (number == 3)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpPadSizeIncrease, block.transform.position)
        );
    }
    // negative powerups should spawn more often
    else if (number > 3 && number < 8)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpConfuse, block.transform.position)
        );
    }
    else if (number > 7 && number < 12)
    {
        powerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpChaos, block.transform.position)
        );
    }
}

bool IsOtherPowerUpActive(std::vector<PowerUpController*>& powerUps, PowerUpType type)
{
    for (const PowerUpController* powerUp : powerUps)
    {
        if (powerUp->activated && powerUp->type == type)
            return true;
    }
    return false;
}

void Breakout::UpdatePowerUps()
{
    auto ballController = Ball->GetComponent<BallController>();
    auto ballRenderer = Ball->GetComponent<SpriteRenderer>();

    for (PowerUpController* powerUp : powerUps)
    {
        powerUp->rigidbody->Update();
        if (powerUp->activated)
        {
            powerUp->duration -= Game::GetDeltaTime();

            if (powerUp->duration <= 0.0f)
            {
                // remove powerup from list (will later be removed)
                powerUp->activated = false;
                // deactivate effects
                // only reset if no other PowerUp of same type is active
                switch (powerUp->type)
                {
                case PowerUpType::STICKY:
                    if (!IsOtherPowerUpActive(powerUps, PowerUpType::STICKY))
                    {	
                        ballController->sticky = false;
                        playerController->renderer->color = glm::vec3(1.0f);
                    }
                    break;
                case PowerUpType::PASS_THROUGH:
                    if (!IsOtherPowerUpActive(powerUps, PowerUpType::PASS_THROUGH))
                    {
                        ballController->passThrough = false;
                        ballRenderer->color = glm::vec3(1.0f);
                    }
                    break;
                case PowerUpType::CONFUSE:
                    if (!IsOtherPowerUpActive(powerUps, PowerUpType::CONFUSE))
                    {
                        postProcessData->confuse = false;
                    }
                    break;
                case PowerUpType::CHAOS:
                    if (!IsOtherPowerUpActive(powerUps, PowerUpType::CHAOS))
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

    powerUps.erase(std::remove_if(powerUps.begin(),powerUps.end(),
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