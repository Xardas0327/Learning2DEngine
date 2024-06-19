#include "Breakout.h"

#include <glm/glm.hpp>
#include <irrklang/irrKlang.h>

#include <sstream> 

#include "Learning2DEngine/Render/RenderManager.h"
#include "Learning2DEngine/Render/Text2DRenderer.h"
#include "Learning2DEngine/System/ResourceManager.h"
#include "Learning2DEngine/System/Random.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "ball_object.h"
#include "particle_generator.h"
#include "post_processor.h"


using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;
using namespace irrklang;

// Game-related State data
SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;
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
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
    SoundEngine->drop();
}

void Breakout::Init()
{
    Game::Init();

    const Resolution resolution = RenderManager::GetInstance().GetResolution();
    const int middleHeight = resolution.GetHeight() / 2;

    auto& resourceManager = ResourceManager::GetInstance();
    // load shaders
    resourceManager.LoadShader("Shaders/sprite.vs", "Shaders/sprite.frag", nullptr, "sprite");
    resourceManager.LoadShader("Shaders/particle.vs", "Shaders/particle.frag", nullptr, "particle");
    resourceManager.LoadShader("Shaders/post_processing.vs", "Shaders/post_processing.frag", nullptr, "postprocessing");
    // configure shaders
    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(resolution.GetWidth()),
        static_cast<float>(resolution.GetHeight()),
        0.0f,
        -1.0f,
        1.0f
    );
    auto sprite = resourceManager.GetShader("sprite");
    sprite.Use();
    sprite.SetInteger("image", 0);
    sprite.SetMatrix4("projection", projection);
    auto particle = resourceManager.GetShader("particle");
    particle.Use();
    particle.SetInteger("sprite", 0);
    particle.SetMatrix4("projection", projection);
    // load textures
    resourceManager.LoadTexture("Assets/Images/background.jpg", false, "background");
    resourceManager.LoadTexture("Assets/Images/awesomeface.png", true, "face");
    resourceManager.LoadTexture("Assets/Images/block.png", false, "block");
    resourceManager.LoadTexture("Assets/Images/block_solid.png", false, "block_solid");
    resourceManager.LoadTexture("Assets/Images/paddle.png", true, "paddle");
    resourceManager.LoadTexture("Assets/Images/particle.png", true, "particle");
    resourceManager.LoadTexture("Assets/Images/powerup_speed.png", true, "powerup_speed");
    resourceManager.LoadTexture("Assets/Images/powerup_sticky.png", true, "powerup_sticky");
    resourceManager.LoadTexture("Assets/Images/powerup_increase.png", true, "powerup_increase");
    resourceManager.LoadTexture("Assets/Images/powerup_confuse.png", true, "powerup_confuse");
    resourceManager.LoadTexture("Assets/Images/powerup_chaos.png", true, "powerup_chaos");
    resourceManager.LoadTexture("Assets/Images/powerup_passthrough.png", true, "powerup_passthrough");
    // set render-specific controls
    Renderer = new SpriteRenderer(sprite);
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
    Player = new GameObject(playerPos, PLAYER_SIZE, resourceManager.GetTexture("paddle"));

    //Ball
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, resourceManager.GetTexture("face"));

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

void Breakout::ProcessInput(float deltaTime)
{
    if (inputKeys[GLFW_KEY_ESCAPE] == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    if (this->State == GAME_MENU)
    {
        if (inputKeys[GLFW_KEY_ENTER] == InputStatus::KEY_DOWN)
        {
            this->State = GAME_ACTIVE;
        }

        if (inputKeys[GLFW_KEY_W] == InputStatus::KEY_DOWN)
        {
            this->Level = (this->Level + 1) % 4;
        }
        if (inputKeys[GLFW_KEY_S] == InputStatus::KEY_DOWN)
        {
            if (this->Level > 0)
                --this->Level;
            else
                this->Level = 3;
        }
    }
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * deltaTime;
        // move playerboard
        if (inputKeys[GLFW_KEY_A])
        {
            if (Player->transform.position.x >= 0.0f)
            {
                Player->transform.position.x -= velocity;
                if (Ball->Stuck)
                    Ball->transform.position.x -= velocity;
            }
        }
        if (inputKeys[GLFW_KEY_D])
        {
            if (Player->transform.position.x <= RenderManager::GetInstance().GetResolution().GetWidth() - Player->transform.size.x)
            {
                Player->transform.position.x += velocity;
                if (Ball->Stuck)
                    Ball->transform.position.x += velocity;
            }
        }
        if (inputKeys[GLFW_KEY_SPACE])
            Ball->Stuck = false;
    }

    if (this->State == GAME_WIN)
    {
        if (inputKeys[GLFW_KEY_ENTER] == InputStatus::KEY_DOWN)
        {
            Effects->Chaos = false;
            this->State = GAME_MENU;
        }
    }
}

void Breakout::Update(float deltaTime)
{
    ProcessInput(deltaTime);

    const Resolution resolution = RenderManager::GetInstance().GetResolution();

    Ball->Move(deltaTime, resolution.GetWidth());
    DoCollisions();
    Particles->Update(deltaTime, *Ball, 1, glm::vec2(Ball->Radius / 2.0f));
    UpdatePowerUps(deltaTime);

    if (ShakeTime > 0.0f)
    {
        ShakeTime -= deltaTime;
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
        auto backgroundTexture = ResourceManager::GetInstance().GetTexture("background");
        Renderer->DrawSprite(
            backgroundTexture, 
            glm::vec2(0.0f, 0.0f),
            RenderManager::GetInstance().GetResolution().ToVec2(),
            0.0f);
        // Draw level
        this->Levels[this->Level].Draw(*Renderer);
        // Draw player
        Player->Draw(*Renderer);
        // Draw PowerUps
        for (PowerUp& powerUp : this->PowerUps)
            if (!powerUp.Destroyed)
                powerUp.Draw(*Renderer);
        // Draw Particles
        Particles->Draw();
        // Draw Player
        Ball->Draw(*Renderer);
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

void ActivatePowerUp(PowerUp& powerUp)
{
    if (powerUp.Type == "speed")
    {
        Ball->Velocity *= 1.2;
    }
    else if (powerUp.Type == "sticky")
    {
        Ball->Sticky = true;
        Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp.Type == "pass-through")
    {
        Ball->PassThrough = true;
        Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp.Type == "pad-size-increase")
    {
        Player->transform.size.x += 50;
    }
    else if (powerUp.Type == "confuse")
    {
        if (!Effects->Chaos)
            Effects->Confuse = true; // only activate if chaos wasn't already active
    }
    else if (powerUp.Type == "chaos")
    {
        if (!Effects->Confuse)
            Effects->Chaos = true;
    }
}

bool CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.transform.position.x + one.transform.size.x >= two.transform.position.x &&
        two.transform.position.x + two.transform.size.x >= one.transform.position.x;
    // collision y-axis?
    bool collisionY = one.transform.position.y + one.transform.size.y >= two.transform.position.y &&
        two.transform.position.y + two.transform.size.y >= one.transform.position.y;
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

Collision CheckCollision(BallObject& one, GameObject& two) // AABB - Circle collision
{
    // get center point circle first 
    glm::vec2 center(one.transform.position + one.Radius);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.transform.size.x / 2.0f, two.transform.size.y / 2.0f);
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

    if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

void Breakout::DoCollisions()
{
    for (GameObject& box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) // if collision is true
            {
                // destroy block if not solid
                if (!box.IsSolid)
                {
                    box.Destroyed = true;
                    this->SpawnPowerUps(box);
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
                if (!(Ball->PassThrough && !box.IsSolid)) // don't do collision resolution on non-solid bricks if pass-through is activated
                {
                    if (dir == LEFT || dir == RIGHT) // horizontal collision
                    {
                        Ball->Velocity.x = -Ball->Velocity.x; // reverse horizontal velocity
                        // relocate
                        float penetration = Ball->Radius - std::abs(diff_vector.x);
                        if (dir == LEFT)
                            Ball->transform.position.x += penetration; // move ball to right
                        else
                            Ball->transform.position.x -= penetration; // move ball to left;
                    }
                    else // vertical collision
                    {
                        Ball->Velocity.y = -Ball->Velocity.y; // reverse vertical velocity
                        // relocate
                        float penetration = Ball->Radius - std::abs(diff_vector.y);
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
    for (PowerUp& powerUp : this->PowerUps)
    {
        if (!powerUp.Destroyed)
        {
            // first check if powerup passed bottom edge, if so: keep as inactive and destroy
            if (powerUp.transform.position.y >= RenderManager::GetInstance().GetResolution().GetHeight())
                powerUp.Destroyed = true;

            if (CheckCollision(*Player, powerUp))
            {	// collided with player, now activate powerup
                ActivatePowerUp(powerUp);
                powerUp.Destroyed = true;
                powerUp.Activated = true;
                SoundEngine->play2D("Assets/Sounds/powerup.wav", false);
            }
        }
    }
    // check collisions for player pad (unless stuck)
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = Player->transform.position.x + Player->transform.size.x / 2.0f;
        float distance = (Ball->transform.position.x + Ball->Radius) - centerBoard;
        float percentage = distance / (Player->transform.size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        //Ball->Velocity.y = -Ball->Velocity.y;
        // keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
        // fix sticky paddle
        Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
        Ball->Stuck = Ball->Sticky;

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
    Player->transform.size = PLAYER_SIZE;
    Player->transform.position = glm::vec2(resolution.GetWidth() / 2.0f - PLAYER_SIZE.x / 2.0f, resolution.GetHeight() - PLAYER_SIZE.y);
    Ball->Reset(Player->transform.position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);

    // also disable all active powerups
    Effects->Chaos = Effects->Confuse = false;
    Ball->PassThrough = Ball->Sticky = false;
    Player->Color = glm::vec3(1.0f);
    Ball->Color = glm::vec3(1.0f);
}

bool ShouldSpawn(int chance)
{
    int random = Random::GetNumber(0, chance);
    return random == 0;
}

void Breakout::SpawnPowerUps(GameObject& block)
{
    auto& resourceManager = ResourceManager::GetInstance();

    if (ShouldSpawn(75)) // 1 in 75 chance
        PowerUps.push_back(
            PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.transform.position, resourceManager.GetTexture("powerup_speed")
            ));
    if (ShouldSpawn(75))
        PowerUps.push_back(
            PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.transform.position, resourceManager.GetTexture("powerup_sticky")
            ));
    if (ShouldSpawn(75))
        PowerUps.push_back(
            PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.transform.position, resourceManager.GetTexture("powerup_passthrough")
            ));
    if (ShouldSpawn(75))
        PowerUps.push_back(
            PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.transform.position, resourceManager.GetTexture("powerup_increase")
            ));
    if (ShouldSpawn(15)) // negative powerups should spawn more often
        PowerUps.push_back(
            PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.transform.position, resourceManager.GetTexture("powerup_confuse")
            ));
    if (ShouldSpawn(15))
        PowerUps.push_back(
            PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.transform.position, resourceManager.GetTexture("powerup_chaos")
            ));
}

bool IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type)
{
    for (const PowerUp& powerUp : powerUps)
    {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return true;
    }
    return false;
}

void Breakout::UpdatePowerUps(float deltaTime)
{
    for (PowerUp& powerUp : this->PowerUps)
    {
        powerUp.transform.position += powerUp.Velocity * deltaTime;
        if (powerUp.Activated)
        {
            powerUp.Duration -= deltaTime;

            if (powerUp.Duration <= 0.0f)
            {
                // remove powerup from list (will later be removed)
                powerUp.Activated = false;
                // deactivate effects
                if (powerUp.Type == "sticky")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
                    {	// only reset if no other PowerUp of type sticky is active
                        Ball->Sticky = false;
                        Player->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "pass-through")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
                    {	// only reset if no other PowerUp of type pass-through is active
                        Ball->PassThrough = false;
                        Ball->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "confuse")
                {
                    if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
                    {	// only reset if no other PowerUp of type confuse is active
                        Effects->Confuse = false;
                    }
                }
                else if (powerUp.Type == "chaos")
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
        [](const PowerUp& powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
    ), this->PowerUps.end());
}