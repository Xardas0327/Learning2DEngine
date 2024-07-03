#include "Breakout.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <irrklang/irrKlang.h>

#include "Learning2DEngine/Render/RenderManager.h"
#include "Learning2DEngine/Render/SpriteRenderer.h"
#include "Learning2DEngine/Render/Text2DRenderer.h"
#include "Learning2DEngine/System/ResourceManager.h"
#include "Learning2DEngine/System/Random.h"
#include "Learning2DEngine/System/GameObject.h"

#include "BallController.h"
#include "BrickController.h"
#include "PowerUpType.h"
#include "PowerUpObject.h"

#include "particle_generator.h"
#include "post_processor.h"


using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace irrklang;

// Game-related State data
GameObject* Background;
GameObject* Player;
GameObject* Ball;
ParticleGenerator* Particles;
PostProcessor* Effects;
ISoundEngine* SoundEngine = createIrrKlangDevice();

float ShakeTime = 0.0f;

const FontSizePair fontSizePair("Assets/Fonts/OCRAEXT.TTF", 24);

Breakout::Breakout() :
    State(GAME_ACTIVE), Level(0), Lives(3)
{

}

Breakout::~Breakout()
{
    delete Background;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
    SoundEngine->drop();
}

void Breakout::Init()
{
    Game::Init();

    auto& resourceManager = ResourceManager::GetInstance();
    const Resolution resolution = RenderManager::GetInstance().GetResolution();
    const int middleHeight = resolution.GetHeight() / 2;

    // Load shaders
    resourceManager.LoadShader("Shaders/particle.vs", "Shaders/particle.frag", nullptr, "particle");
    resourceManager.LoadShader("Shaders/post_processing.vs", "Shaders/post_processing.frag", nullptr, "postprocessing");

    // Configure shaders
    Game::cameraProjection = glm::ortho(
        0.0f,
        static_cast<float>(resolution.GetWidth()),
        static_cast<float>(resolution.GetHeight()),
        0.0f,
        -1.0f,
        1.0f
    );

    auto particle = resourceManager.GetShader("particle");
    particle.Use();
    particle.SetInteger("sprite", 0);
    particle.SetMatrix4("projection", cameraProjection);

    // Load textures
    Texture2DSettings alphaSettings;
    alphaSettings.internalFormat = GL_RGBA;
    alphaSettings.imageFormat = GL_RGBA;
    Texture2DSettings nonAlphaSettings;

    resourceManager.LoadTexture("Assets/Images/background.jpg", nonAlphaSettings, "background");
    resourceManager.LoadTexture("Assets/Images/awesomeface.png", alphaSettings, "face");
    resourceManager.LoadTexture("Assets/Images/block.png", nonAlphaSettings, "block");
    resourceManager.LoadTexture("Assets/Images/block_solid.png", nonAlphaSettings, "block_solid");
    resourceManager.LoadTexture("Assets/Images/paddle.png", alphaSettings, "paddle");
    resourceManager.LoadTexture("Assets/Images/particle.png", alphaSettings, "particle");
    resourceManager.LoadTexture("Assets/Images/powerup_speed.png", alphaSettings, "powerup_speed");
    resourceManager.LoadTexture("Assets/Images/powerup_sticky.png", alphaSettings, "powerup_sticky");
    resourceManager.LoadTexture("Assets/Images/powerup_increase.png", alphaSettings, "powerup_increase");
    resourceManager.LoadTexture("Assets/Images/powerup_confuse.png", alphaSettings, "powerup_confuse");
    resourceManager.LoadTexture("Assets/Images/powerup_chaos.png", alphaSettings, "powerup_chaos");
    resourceManager.LoadTexture("Assets/Images/powerup_passthrough.png", alphaSettings, "powerup_passthrough");

    // Background
    Background = new GameObject(
        Transform(
            glm::vec2(0.0f, 0.0f),
            resolution.ToVec2(),
            0.0f
        )
    );
    Background->AddComponent<SpriteRenderer, const Texture2D&>(
        resourceManager.GetTexture("background")
    );

    // Particles
    Particles = new ParticleGenerator(
        resourceManager.GetShader("particle"),
        resourceManager.GetTexture("particle"),
        2000
    );

    // PostProcessor
    Effects = new PostProcessor(resourceManager.GetShader("postprocessing"), resolution.GetWidth(), resolution.GetHeight());

    // Load levels
    GameLevel one; 
    one.Load("Assets/Levels/one.lvl", resolution.GetWidth(), middleHeight);
    GameLevel two; 
    two.Load("Assets/Levels/two.lvl", resolution.GetWidth(), middleHeight);
    GameLevel three; 
    three.Load("Assets/Levels/three.lvl", resolution.GetWidth(), middleHeight);
    GameLevel four; 
    four.Load("Assets/Levels/four.lvl", resolution.GetWidth(), middleHeight);

    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;

    // Player
    glm::vec2 playerPos = glm::vec2(resolution.GetWidth() / 2.0f - PLAYER_SIZE.x / 2.0f, resolution.GetHeight() - PLAYER_SIZE.y);
    Player = new GameObject(
        Transform(
            playerPos,
            PLAYER_SIZE
        )
    );
    Player->AddComponent<SpriteRenderer, const Texture2D&>(
        resourceManager.GetTexture("paddle")
    );

    // Ball
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new GameObject(
        Transform(
            ballPos,
            glm::vec2(BALL_RADIUS * 2.0f, BALL_RADIUS * 2.0f)
        )
    );
    Ball->AddComponent<SpriteRenderer, const Texture2D&>(
        resourceManager.GetTexture("face")
    );
    auto ballController = Ball->AddComponent<BallController, float, glm::vec2>(BALL_RADIUS, INITIAL_BALL_VELOCITY);

    // Sounds
    SoundEngine->play2D("Assets/Sounds/breakout.mp3", true);

    // Text
    auto& textRenderer = Text2DRenderer::GetInstance();
    textRenderer.Load(fontSizePair);

    liveText = { 
        fontSizePair,
        "Lives: " + std::to_string(this->Lives),
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
    State = GAME_MENU;
}

void Breakout::ProcessInput()
{
    if (Game::inputKeys[GLFW_KEY_ESCAPE] == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    if (this->State == GAME_MENU)
    {
        if (Game::inputKeys[GLFW_KEY_ENTER] == InputStatus::KEY_DOWN)
        {
            this->State = GAME_ACTIVE;
        }

        if (Game::inputKeys[GLFW_KEY_W] == InputStatus::KEY_DOWN)
        {
            this->Level = (this->Level + 1) % 4;
        }
        if (Game::inputKeys[GLFW_KEY_S] == InputStatus::KEY_DOWN)
        {
            if (this->Level > 0)
                --this->Level;
            else
                this->Level = 3;
        }
    }
    if (this->State == GAME_ACTIVE)
    {
        auto ballController = Ball->GetComponent<BallController>();
        float velocity = PLAYER_VELOCITY * Game::deltaTime;
        // move playerboard
        if (Game::inputKeys[GLFW_KEY_A])
        {
            if (Player->transform.position.x >= 0.0f)
            {
                Player->transform.position.x -= velocity;
                if (ballController->stuck)
                    Ball->transform.position.x -= velocity;
            }
        }
        if (Game::inputKeys[GLFW_KEY_D])
        {
            if (Player->transform.position.x <= RenderManager::GetInstance().GetResolution().GetWidth() - Player->transform.scale.x)
            {
                Player->transform.position.x += velocity;
                if (ballController->stuck)
                    Ball->transform.position.x += velocity;
            }
        }
        if (Game::inputKeys[GLFW_KEY_SPACE])
            ballController->stuck = false;
    }

    if (this->State == GAME_WIN)
    {
        if (Game::inputKeys[GLFW_KEY_ENTER] == InputStatus::KEY_DOWN)
        {
            Effects->Chaos = false;
            this->State = GAME_MENU;
        }
    }
}

void Breakout::Update()
{
    ProcessInput();

    const Resolution resolution = RenderManager::GetInstance().GetResolution();
    auto ballController = Ball->GetComponent<BallController>();

    ballController->Move(Game::deltaTime, resolution.GetWidth());
    DoCollisions();
    Particles->Update(Game::deltaTime, *ballController, 1, glm::vec2(ballController->radius / 2.0f));
    UpdatePowerUps();

    if (ShakeTime > 0.0f)
    {
        ShakeTime -= Game::deltaTime;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }

    // Lose live
    if (Ball->transform.position.y >= resolution.GetHeight())
    {
        --Lives;
        // Game over
        if (Lives == 0)
        {
            ResetLevel();
            State = GAME_MENU;
        }
        else
        {
            liveText.text = "Lives: " + std::to_string(Lives);
        }
        ResetPlayer();
        ClearPowerUps();
    }

    // check win condition
    if (State == GAME_ACTIVE && Levels[Level].IsCompleted())
    {
        ResetLevel();
        ResetPlayer();
        ClearPowerUps();
        Effects->Chaos = true;
        State = GAME_WIN;
    }
}

void Breakout::Render()
{
    auto& textRenderer = Text2DRenderer::GetInstance();
    // Begin rendering to postprocessing framebuffer
    Effects->BeginRender();

    // Draw background
    Background->GetComponent<SpriteRenderer>()->Draw();
    // Draw level
    this->Levels[this->Level].Draw();
    // Draw player
    Player->GetComponent<SpriteRenderer>()->Draw();
    // Draw PowerUps
    for (PowerUpController* powerUp : this->PowerUps)
    {
        if (powerUp->gameObject->isActive)
            powerUp->gameObject->GetComponent<SpriteRenderer>()->Draw();
    }
    // Draw Particles
    Particles->Draw();
    // Draw Player
    Ball->GetComponent<SpriteRenderer>()->Draw();
    // End rendering to postprocessing framebuffer
    Effects->EndRender();
    // Render postprocessing quad
    Effects->Render(glfwGetTime());

    textRenderer.RenderText(liveText);

    if (this->State == GAME_MENU)
    {
        textRenderer.RenderText(startText);
        textRenderer.RenderText(levelSelectorText);
    }

    if (this->State == GAME_WIN)
    {
        textRenderer.RenderText(winText);
        textRenderer.RenderText(retryText);
    }
}

void ActivatePowerUp(PowerUpController& powerUp)
{
    auto ballController = Ball->GetComponent<BallController>();
    auto ballRenderer = Ball->GetComponent<SpriteRenderer>();
    auto playerRenderer = Player->GetComponent<SpriteRenderer>();

    switch (powerUp.type)
    {
    case PowerUpType::SPEED:
        ballController->velocity *= 1.2;
        break;
    case PowerUpType::STICKY:
        ballController->sticky = true;
        playerRenderer->color = glm::vec3(1.0f, 0.5f, 1.0f);
        break;
    case PowerUpType::PASS_THROUGH:
        ballController->passThrough = true;
        ballRenderer->color = glm::vec3(1.0f, 0.5f, 0.5f);
        break;
    case PowerUpType::PAD_SIZE_INCREASE:
        Player->transform.scale.x += 50;
        break;
    case PowerUpType::CONFUSE:
        // only activate if chaos wasn't already active
        if (!Effects->Chaos)
            Effects->Confuse = true;
        break;
    case PowerUpType::CHAOS:
        if (!Effects->Confuse)
            Effects->Chaos = true;
        break;
    default:
        break;
    }
}

bool CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.transform.position.x + one.transform.scale.x >= two.transform.position.x &&
        two.transform.position.x + two.transform.scale.x >= one.transform.position.x;
    // collision y-axis?
    bool collisionY = one.transform.position.y + one.transform.scale.y >= two.transform.position.y &&
        two.transform.position.y + two.transform.scale.y >= one.transform.position.y;
    // collision only if on both axes
    return collisionX && collisionY;
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

Collision CheckCollision(BallController& one, GameObject& two) // AABB - Circle collision
{
    // get center point circle first 
    glm::vec2 center(one.gameObject->transform.position + one.radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.transform.scale.x / 2.0f, two.transform.scale.y / 2.0f);
    glm::vec2 aabb_center(
        two.transform.position.x + aabb_half_extents.x,
        two.transform.position.y + aabb_half_extents.y
    );
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;

    if (glm::length(difference) < one.radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

void Breakout::DoCollisions()
{
    auto ballController = Ball->GetComponent<BallController>();

    for (BrickController* box : this->Levels[this->Level].bricks)
    {
        if (box->gameObject->isActive)
        {
            Collision collision = CheckCollision(*ballController, *box->gameObject);
            if (std::get<0>(collision)) // if collision is true
            {
                // destroy block if not solid
                if (!box->isSolid)
                {
                    box->gameObject->isActive = false;
                    this->SpawnPowerUps(*box->gameObject);
                    SoundEngine->play2D("Assets/Sounds/bleep.mp3", false);
                }
                else
                {   // if block is solid, enable shake effect
                    ShakeTime = 0.05f;
                    Effects->Shake = true;
                    SoundEngine->play2D("Assets/Sounds/solid.wav", false);
                }
                // collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (!(ballController->passThrough && !box->isSolid)) // don't do collision resolution on non-solid bricks if pass-through is activated
                {
                    if (dir == LEFT || dir == RIGHT) // horizontal collision
                    {
                        ballController->velocity.x = -ballController->velocity.x; // reverse horizontal velocity
                        // relocate
                        float penetration = ballController->radius - std::abs(diff_vector.x);
                        if (dir == LEFT)
                            Ball->transform.position.x += penetration; // move ball to right
                        else
                            Ball->transform.position.x -= penetration; // move ball to left;
                    }
                    else // vertical collision
                    {
                        ballController->velocity.y = -ballController->velocity.y; // reverse vertical velocity
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
    for (PowerUpController* powerUp : this->PowerUps)
    {
        if (powerUp->gameObject->isActive)
        {
            // first check if powerup passed bottom edge, if so: keep as inactive and destroy
            if (powerUp->gameObject->transform.position.y >= RenderManager::GetInstance().GetResolution().GetHeight())
                powerUp->gameObject->isActive = false;

            if (CheckCollision(*Player, *(powerUp->gameObject)))
            {	// collided with player, now activate powerup
                ActivatePowerUp(*powerUp);
                powerUp->gameObject->isActive = false;
                powerUp->activated = true;
                SoundEngine->play2D("Assets/Sounds/powerup.wav", false);
            }
        }
    }
    // check collisions for player pad (unless stuck)
    Collision result = CheckCollision(*ballController, *Player);
    if (!ballController->stuck && std::get<0>(result))
    {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = Player->transform.position.x + Player->transform.scale.x / 2.0f;
        float distance = (Ball->transform.position.x + ballController->radius) - centerBoard;
        float percentage = distance / (Player->transform.scale.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = ballController->velocity;
        ballController->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        //Ball->Velocity.y = -Ball->Velocity.y;
        // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        ballController->velocity = glm::normalize(ballController->velocity) * glm::length(oldVelocity);
        // fix sticky paddle
        ballController->velocity.y = -1.0f * abs(ballController->velocity.y);
        ballController->stuck = ballController->sticky;

        SoundEngine->play2D("Assets/Sounds/bleep.wav", false);
    }
}

void Breakout::ResetLevel()
{
    const Resolution resolution = RenderManager::GetInstance().GetResolution();
    const int middleHeight = resolution.GetHeight() / 2;

    if (Level == 0)
        Levels[0].Load("Assets/Levels/one.lvl", resolution.GetWidth(), middleHeight);
    else if (Level == 1)
        Levels[1].Load("Assets/Levels/two.lvl", resolution.GetWidth(), middleHeight);
    else if (Level == 2)
        Levels[2].Load("Assets/Levels/three.lvl", resolution.GetWidth(), middleHeight);
    else if (Level == 3)
        Levels[3].Load("Assets/Levels/four.lvl", resolution.GetWidth(), middleHeight);

    Lives = 3;
    liveText.text = "Lives: " + std::to_string(Lives);
}

void Breakout::ResetPlayer()
{
    const Resolution resolution = RenderManager::GetInstance().GetResolution();

    // reset player/ball stats
    Player->transform.scale = PLAYER_SIZE;
    Player->transform.position = glm::vec2(resolution.GetWidth() / 2.0f - PLAYER_SIZE.x / 2.0f, resolution.GetHeight() - PLAYER_SIZE.y);
    auto ballController = Ball->GetComponent<BallController>();
    ballController->Reset(Player->transform.position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);

    // also disable all active powerups
    Effects->Chaos = false;
    Effects->Confuse = false;
    ballController->passThrough = false;
    ballController->sticky = false;

    auto playerRenderer = Player->GetComponent<SpriteRenderer>();
    playerRenderer->color = glm::vec3(1.0f);
    auto ballRenderer = Ball->GetComponent<SpriteRenderer>();
    ballRenderer->color = glm::vec3(1.0f);
}

void Breakout::SpawnPowerUps(GameObject& block)
{
    int number = Random::GetNumber(0, 50);
    if (number == 0)
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpSpeed, block.transform.position)
        );
    }
    else if (number == 1)
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpSticky, block.transform.position)
        );
    }
    else if (number == 2)
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpPassThrough, block.transform.position)
        );
    }
    else if (number == 3)
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpPadSizeIncrease, block.transform.position)
        );
    }
    // negative powerups should spawn more often
    else if (number > 3 && number < 8)
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp(PowerUpConfuse, block.transform.position)
        );
    }
    else if (number > 7 && number < 12)
    {
        PowerUps.push_back(
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

    for (PowerUpController* powerUp : this->PowerUps)
    {
        powerUp->gameObject->transform.position += powerUp->velocity * Game::deltaTime;
        if (powerUp->activated)
        {
            powerUp->duration -= Game::deltaTime;

            if (powerUp->duration <= 0.0f)
            {
                // remove powerup from list (will later be removed)
                powerUp->activated = false;
                // deactivate effects
                // only reset if no other PowerUp of same type is active
                switch (powerUp->type)
                {
                case PowerUpType::STICKY:
                    if (!IsOtherPowerUpActive(this->PowerUps, PowerUpType::STICKY))
                    {	
                        ballController->sticky = false;

                        auto playerRenderer = Player->GetComponent<SpriteRenderer>();
                        playerRenderer->color = glm::vec3(1.0f);
                    }
                    break;
                case PowerUpType::PASS_THROUGH:
                    if (!IsOtherPowerUpActive(this->PowerUps, PowerUpType::PASS_THROUGH))
                    {
                        ballController->passThrough = false;
                        ballRenderer->color = glm::vec3(1.0f);
                    }
                    break;
                case PowerUpType::CONFUSE:
                    if (!IsOtherPowerUpActive(this->PowerUps, PowerUpType::CONFUSE))
                    {
                        Effects->Confuse = false;
                    }
                    break;
                case PowerUpType::CHAOS:
                    if (!IsOtherPowerUpActive(this->PowerUps, PowerUpType::CHAOS))
                    {
                        Effects->Chaos = false;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](PowerUpController* powerUp)
        {
            bool isDeletable = !powerUp->gameObject->isActive && !powerUp->activated;
            if (isDeletable)
            {
                GameObject::Destroy(powerUp);
            }

            return isDeletable;
        }
    ), this->PowerUps.end());
}

void Breakout::ClearPowerUps()
{
    for (PowerUpController* powerUp : PowerUps)
    {
        GameObject::Destroy(powerUp);
    }
    PowerUps.clear();
}