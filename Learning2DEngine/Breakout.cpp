#include "Breakout.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <irrklang/irrKlang.h>

#include <sstream> 

#include "Learning2DEngine/Render/RenderManager.h"
#include "Learning2DEngine/Render/SpriteRenderer.h"
#include "Learning2DEngine/Render/Text2DRenderer.h"
#include "Learning2DEngine/System/ResourceManager.h"
#include "Learning2DEngine/System/Random.h"
#include "Learning2DEngine/System/GameObject.h"

#include "BallController.h"

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

    // load shaders
    resourceManager.LoadShader("Shaders/particle.vs", "Shaders/particle.frag", nullptr, "particle");
    resourceManager.LoadShader("Shaders/post_processing.vs", "Shaders/post_processing.frag", nullptr, "postprocessing");
    // configure shaders
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

    Texture2DSettings alphaSettings;
    alphaSettings.internalFormat = GL_RGBA;
    alphaSettings.imageFormat = GL_RGBA;
    Texture2DSettings nonAlphaSettings;

    // load textures
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
    // set render-specific controls
    Background = new GameObject(
        Transform(
            glm::vec2(0.0f, 0.0f),
            resolution.ToVec2(),
            0.0f
        )
    );
    auto backgroundRenderer = Background->AddRenderer<SpriteRenderer>();
    backgroundRenderer->texture = new Texture2D(resourceManager.GetTexture("background"));

    Particles = new ParticleGenerator(
        resourceManager.GetShader("particle"),
        resourceManager.GetTexture("particle"),
        2000
    );
    Effects = new PostProcessor(resourceManager.GetShader("postprocessing"), resolution.GetWidth(), resolution.GetHeight());
    // load levels
    GameLevel one; one.Load("Assets/Levels/one.lvl", resolution.GetWidth(), middleHeight);
    GameLevel two; two.Load("Assets/Levels/two.lvl", resolution.GetWidth(), middleHeight);
    GameLevel three; three.Load("Assets/Levels/three.lvl", resolution.GetWidth(), middleHeight);
    GameLevel four; four.Load("Assets/Levels/four.lvl", resolution.GetWidth(), middleHeight);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;

    //Player
    glm::vec2 playerPos = glm::vec2(resolution.GetWidth() / 2.0f - PLAYER_SIZE.x / 2.0f, resolution.GetHeight() - PLAYER_SIZE.y);
    Player = new GameObject(
        Transform(
            playerPos,
            PLAYER_SIZE
        )
    );
    auto playerRenderer = Player->AddRenderer<SpriteRenderer>();
    playerRenderer->texture = new Texture2D(resourceManager.GetTexture("paddle"));

    //Ball
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new GameObject(
        Transform(
            ballPos,
            glm::vec2(BALL_RADIUS * 2.0f, BALL_RADIUS * 2.0f)
        )
    );
    auto ballRenderer = Ball->AddRenderer<SpriteRenderer>();
    ballRenderer->texture = new Texture2D(resourceManager.GetTexture("face"));
    auto ballController = Ball->AddBehaviour<BallController, float, glm::vec2>(BALL_RADIUS, INITIAL_BALL_VELOCITY);

    //Sounds
    SoundEngine->play2D("Assets/Sounds/breakout.mp3", true);

    //Text
    auto& textRenderer = Text2DRenderer::GetInstance();
    textRenderer.Init();
    textRenderer.Load(fontSizePair);

    liveText = { 
        fontSizePair,
        "Lives: 0",
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

    //State
    State = GAME_MENU;

    //Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Breakout::Terminate()
{
    ResourceManager::GetInstance().Clear();
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
        auto ballController = Ball->GetBehaviour<BallController>();
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
    auto ballController = Ball->GetBehaviour<BallController>();

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
        ResetPlayer();
    }

    // check win condition
    if (State == GAME_ACTIVE && Levels[Level].IsCompleted())
    {
        ResetLevel();
        ResetPlayer();
        Effects->Chaos = true;
        State = GAME_WIN;
    }
}

void Breakout::Render()
{
    auto& textRenderer = Text2DRenderer::GetInstance();
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU || this->State == GAME_WIN)
    {
        // Begin rendering to postprocessing framebuffer
        Effects->BeginRender();

        // Draw background
        Background->GetRenderer()->Draw();
        // Draw level
        this->Levels[this->Level].Draw();
        // Draw player
        Player->GetRenderer()->Draw();
        // Draw PowerUps
        for (PowerUpController* powerUp : this->PowerUps)
        {
            if (powerUp->gameObject->isActive)
                powerUp->gameObject->GetRenderer()->Draw();
        }
        // Draw Particles
        Particles->Draw();
        // Draw Player
        Ball->GetRenderer()->Draw();
        // End rendering to postprocessing framebuffer
        Effects->EndRender();
        // Render postprocessing quad
        Effects->Render(glfwGetTime());

        //Update live
        std::stringstream ss;
        ss << this->Lives;
        liveText.text = "Lives: " + ss.str();
        textRenderer.RenderText(liveText);
    }

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
    auto ballController = Ball->GetBehaviour<BallController>();
    if (powerUp.type == "speed")
    {
        ballController->velocity *= 1.2;
    }
    else if (powerUp.type == "sticky")
    {
        ballController->sticky = true;
        auto playerRenderer = Player->GetRenderer<SpriteRenderer>();
        playerRenderer->color = glm::vec3(1.0f, 0.5f, 1.0f);
        
    }
    else if (powerUp.type == "pass-through")
    {
        ballController->passThrough = true;
        auto ballRenderer = Ball->GetRenderer<SpriteRenderer>();
        ballRenderer->color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp.type == "pad-size-increase")
    {
        Player->transform.scale.x += 50;
    }
    else if (powerUp.type == "confuse")
    {
        if (!Effects->Chaos)
            Effects->Confuse = true; // only activate if chaos wasn't already active
    }
    else if (powerUp.type == "chaos")
    {
        if (!Effects->Confuse)
            Effects->Chaos = true;
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
    auto ballController = Ball->GetBehaviour<BallController>();

    for (BrickController* box : this->Levels[this->Level].Bricks)
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
}

void Breakout::ResetPlayer()
{
    const Resolution resolution = RenderManager::GetInstance().GetResolution();

    // reset player/ball stats
    Player->transform.scale = PLAYER_SIZE;
    Player->transform.position = glm::vec2(resolution.GetWidth() / 2.0f - PLAYER_SIZE.x / 2.0f, resolution.GetHeight() - PLAYER_SIZE.y);
    auto ballController = Ball->GetBehaviour<BallController>();
    ballController->Reset(Player->transform.position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);

    // also disable all active powerups
    Effects->Chaos = Effects->Confuse = false;
    ballController->passThrough = ballController->sticky = false;

    auto playerRenderer = Player->GetRenderer<SpriteRenderer>();
    playerRenderer->color = glm::vec3(1.0f);
    auto ballRenderer = Ball->GetRenderer<SpriteRenderer>();
    ballRenderer->color = glm::vec3(1.0f);
}

bool ShouldSpawn(int chance)
{
    int random = Random::GetNumber(0, chance);
    return random == 0;
}

void Breakout::SpawnPowerUps(GameObject& block)
{
    if (ShouldSpawn(75)) // 1 in 75 chance
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp("speed", 0.0f, block.transform.position, "powerup_speed", glm::vec3(0.5f, 0.5f, 1.0f))
        );
    }
        
    if (ShouldSpawn(75))
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp("sticky", 20.0f, block.transform.position, "powerup_sticky", glm::vec3(1.0f, 0.5f, 1.0f))
        );
    }
    if (ShouldSpawn(75))
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp("pass-through", 10.0f, block.transform.position, "powerup_passthrough", glm::vec3(0.5f, 1.0f, 0.5f))
        );
    }
    if (ShouldSpawn(75))
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp("pad-size-increase", 0.0f, block.transform.position, "powerup_increase", glm::vec3(1.0f, 0.6f, 0.4f))
        );
    }
    if (ShouldSpawn(15)) // negative powerups should spawn more often
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp("confuse", 15.0f, block.transform.position, "powerup_confuse", glm::vec3(1.0f, 0.3f, 0.3f))
        );
    }
    if (ShouldSpawn(15))
    {
        PowerUps.push_back(
            PowerUpController::CreatePowerUp("chaos", 15.0f, block.transform.position, "powerup_chaos", glm::vec3(0.9f, 0.25f, 0.25f))
        );
    }
}

bool IsOtherPowerUpActive(std::vector<PowerUpController*>& powerUps, std::string type)
{
    for (const PowerUpController* powerUp : powerUps)
    {
        if (powerUp->activated)
            if (powerUp->type == type)
                return true;
    }
    return false;
}

void Breakout::UpdatePowerUps()
{
    auto ballController = Ball->GetBehaviour<BallController>();
    auto ballRenderer = Ball->GetRenderer<SpriteRenderer>();

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
                if (powerUp->type == "sticky")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
                    {	// only reset if no other PowerUp of type sticky is active
                        ballController->sticky = false;

                        auto playerRenderer = Player->GetRenderer<SpriteRenderer>();
                        playerRenderer->color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp->type == "pass-through")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
                    {	// only reset if no other PowerUp of type pass-through is active
                        ballController->passThrough = false;
                        ballRenderer->color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp->type == "confuse")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
                    {	// only reset if no other PowerUp of type confuse is active
                        Effects->Confuse = false;
                    }
                }
                else if (powerUp->type == "chaos")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
                    {	// only reset if no other PowerUp of type chaos is active
                        Effects->Chaos = false;
                    }
                }
            }
        }
    }
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUpController* powerUp)
        {
            bool isDeletable = !powerUp->gameObject->isActive && !powerUp->activated;
            if (isDeletable)
            {
                delete powerUp->gameObject;
            }

            return isDeletable;
        }
    ), this->PowerUps.end());
}