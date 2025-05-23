#include "BallController.h"

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/ParticleSimulator/ParticleSystemSettings.h>

#include "BallParticleSettings.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;
using namespace Learning2DEngine::ParticleSimulator;

BallController::BallController(GameObject* gameObject, PlayerController* playerController,
    const std::string& textureId, const std::string& particleTextureId,
    BallHitPlayerEventItem& ballHitPlayerEventItem, BallHitBrickEventItem& ballHitBrickEventItem)
    : CircleColliderComponent(gameObject, BALL_RADIUS, ColliderType::DYNAMIC, ColliderMode::COLLIDER, glm::vec2(0.0f, 0.0f), 0b11),
    UpdaterComponent(gameObject), Component(gameObject),
    textureId(textureId), particleTextureId(particleTextureId), playerController(playerController),
    renderer(nullptr), rigidbody(nullptr), particleSystem(nullptr),
    radius(BALL_RADIUS), isStuck(true), sticky(false), passThrough(false),
    hitPlayerEventHandler(), hitBrickEventHandler()
{
    hitPlayerEventHandler.Add(&ballHitPlayerEventItem);
    hitBrickEventHandler.Add(&ballHitBrickEventItem);
}

void BallController::Init()
{
    CircleColliderComponent::Init();
    UpdaterComponent::Init();

    rigidbody = gameObject->AddComponent<Rigidbody>(INITIAL_BALL_VELOCITY, false, isStuck);
    renderer = gameObject->AddComponent<SpriteRenderComponent>(
        ResourceManager::GetInstance().GetTexture(textureId)
    );

    gameObject->transform.SetScale(glm::vec2(radius * 2.0f, radius * 2.0f));
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

    particleSystem = gameObject->AddComponent<ParticleSystemComponent>(
        500u,
        ResourceManager::GetInstance().GetTexture(particleTextureId),
        ballParticleSystemSettings,
        new BallParticleSettings(glm::vec2(radius / 2.0f), glm::vec2(10.0f, 10.0f), 0.3f, 0.1f),
        300,
        -1
    );
    particleSystem->Start();
}

void BallController::Destroy()
{
    CircleColliderComponent::Destroy();
    UpdaterComponent::Destroy();
}

void BallController::Update()
{
    if (!isStuck)
    {
        int width = Game::mainCamera.GetResolution().GetWidth();

        if (gameObject->transform.GetPosition().x <= 0.0f)
        {
            rigidbody->velocity.x = -rigidbody->velocity.x;
            gameObject->transform.SetPosition(
                glm::vec2(0.0f, gameObject->transform.GetPosition().y)
            );
        }
        else if (gameObject->transform.GetPosition().x + gameObject->transform.GetScale().x >= width)
        {
            rigidbody->velocity.x = -rigidbody->velocity.x;
			gameObject->transform.SetPosition(
				glm::vec2(width - gameObject->transform.GetScale().x, gameObject->transform.GetPosition().y)
			);
        }
        if (gameObject->transform.GetPosition().y <= 0.0f)
        {
            rigidbody->velocity.y = -rigidbody->velocity.y;
			gameObject->transform.SetPosition(
				glm::vec2(gameObject->transform.GetPosition().x, 0.0f)
			);
        }
    }
}

void BallController::Reset()
{
    gameObject->transform.SetPosition(
        playerController->gameObject->transform.GetPosition() + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f)
	);
    rigidbody->velocity = INITIAL_BALL_VELOCITY;
    renderer->data.color = glm::vec4(1.0f);

    SetStuck(true);
    sticky = false;
    passThrough = false;
}

Direction BallController::VectorDirection(glm::vec2 target)
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

void BallController::SetStuck(bool value)
{
    isStuck = value;
    rigidbody->isFrozen = value;
}

void BallController::OnCollision(const Collision& collision)
{
    auto player = collision.collidedObject->GetComponent<PlayerController>();
    if (player != nullptr && !isStuck)
    {
        // check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = collision.collidedObject->transform.GetPosition().x + collision.collidedObject->transform.GetScale().x / 2.0f;
        float distance = (gameObject->transform.GetPosition().x + radius) - centerBoard;
        float percentage = distance / (collision.collidedObject->transform.GetScale().x / 2.0f);

        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = rigidbody->velocity;
        rigidbody->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;

        // keep speed consistent
        rigidbody->velocity = glm::normalize(rigidbody->velocity) * glm::length(oldVelocity);

        // fix sticky paddle
        rigidbody->velocity.y = -1.0f * abs(rigidbody->velocity.y);
        SetStuck(sticky);
        hitPlayerEventHandler.Invoke();
        return;
    }

    auto brick = collision.collidedObject->GetComponent<BrickController>();
    if (brick != nullptr)
    {
        if (!passThrough || brick->IsSolid())
        {
            glm::vec2 difference = collision.edgeOfCollidedObject - GetColliderCenter();
            Direction direction = VectorDirection(difference);
            if (direction == Direction::LEFT || direction == Direction::RIGHT)
            {
                rigidbody->velocity.x = -rigidbody->velocity.x;
            }
            else
            {
                rigidbody->velocity.y = -rigidbody->velocity.y;
            }
        }

        hitBrickEventHandler.Invoke(brick);
    }
}