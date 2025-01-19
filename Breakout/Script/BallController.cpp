#include "BallController.h"

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/ParticleSimulator/ParticleSystemSettings.h>

#include "BallParticleSettings.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;
using namespace Learning2DEngine::ParticleSimulator;

BallController::BallController(GameObject* gameObject, PlayerController* playerController,
    const std::string& textureId, const std::string& particleTextureId)
    : Component(gameObject), textureId(textureId), particleTextureId(particleTextureId), playerController(playerController),
        renderer(nullptr), rigidbody(nullptr), collider(nullptr), particleSystem(nullptr),
        radius(BALL_RADIUS), stuck(true), sticky(false), passThrough(false)
{

}

void BallController::Init()
{
    rigidbody = gameObject->AddComponent<Rigidbody, glm::vec2>(INITIAL_BALL_VELOCITY);
    collider = gameObject->AddComponent<CircleCollider, float>(radius);
    renderer = gameObject->AddComponent<SpriteRenderer, const Texture2D&>(
        ResourceManager::GetInstance().GetTexture(textureId)
    );

    gameObject->transform.scale = glm::vec2(radius * 2.0f, radius * 2.0f);
    Reset();

    InitParticleSystem();
}

void BallController::InitParticleSystem()
{
    ParticleSystemSettings ballParticleSystemSettings(
        true,
        BlendFuncFactor(GL_SRC_ALPHA, GL_ONE),
        0.001f,
        1,
        0.0f
    );

    particleSystem = gameObject->AddComponent<ParticleSystem, unsigned int, const Texture2D&, const ParticleSystemSettings&, ParticleSettings*, int>(
        500,
        ResourceManager::GetInstance().GetTexture(particleTextureId),
        ballParticleSystemSettings,
        new BallParticleSettings(glm::vec2(radius / 2.0f), glm::vec2(10.0f, 10.0f), 0.3f, 0.1f),
        -1
    );
    particleSystem->Start();
}

void BallController::Move()
{
    if (!stuck)
    {
        int width = Game::mainCamera.GetResolution().GetWidth();
        rigidbody->Update();

        if (gameObject->transform.position.x <= 0.0f)
        {
            rigidbody->velocity.x = -rigidbody->velocity.x;
            gameObject->transform.position.x = 0.0f;
        }
        else if (gameObject->transform.position.x + gameObject->transform.scale.x >= width)
        {
            rigidbody->velocity.x = -rigidbody->velocity.x;
            gameObject->transform.position.x = width - gameObject->transform.scale.x;
        }
        if (gameObject->transform.position.y <= 0.0f)
        {
            rigidbody->velocity.y = -rigidbody->velocity.y;
            gameObject->transform.position.y = 0.0f;
        }
    }
}

void BallController::Reset()
{
    gameObject->transform.position = 
        playerController->gameObject->transform.position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    rigidbody->velocity = INITIAL_BALL_VELOCITY;
    renderer->color = glm::vec3(1.0f);

    stuck = true;
    sticky = false;
    passThrough = false;
}