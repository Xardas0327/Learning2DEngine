#include "PlayerController.h"

#include <Learning2DEngine/DebugTool/Log.h>
#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/InputStatus.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>

#include "CoinController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;
using namespace Learning2DEngine::Animator;
#if USE_IRRKLANG_SOUND_ENGINE
using namespace irrklang;
#endif

PlayerController::PlayerController(GameObject* gameObject
#if USE_IRRKLANG_SOUND_ENGINE
    , ISoundEngine* soundEngine
#endif
)
    : UpdaterComponent(gameObject), Component(gameObject),
    BoxColliderComponent(gameObject, PLAYER_SIZE, ColliderType::DYNAMIC, ColliderMode::COLLIDER),
    onGround(false), detector(nullptr), eventItem(this),
    rightIdleAnimation(nullptr), leftIdleAnimation(nullptr), rightRunAnimation(nullptr), leftRunAnimation(nullptr),
    currentState(PlayerAnimatioState::RIGHT_IDLE), rigidbody(nullptr), coinNumber(0), coinCollected()
#if USE_IRRKLANG_SOUND_ENGINE
    , soundEngine(soundEngine)
#endif
{
}

void PlayerController::Init()
{
    UpdaterComponent::Init();
    BoxColliderComponent::Init();

    gameObject->transform.SetScale(PLAYER_SIZE);

    auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
        RendererMode::RENDER,
        ResourceManager::GetInstance().GetTexture(PLAYER_RIGHT_IDLE_TEXTURE_IDS[0])
    );
    rigidbody = gameObject->AddComponent<Rigidbody>(glm::vec2(0.0f, 0.0f), true);
    rigidbody->gravityMultiplier = 50.0f;
    InitRigidbody();

    detector = gameObject->AddComponent<PlatformDetectorController>(glm::vec2(30.0f, 5.0f), glm::vec2(10.0f, 45.0f));
    detector->eventhandler.Add(&eventItem);

    rightIdleAnimation = gameObject->AddComponent<AnimationController>(&renderer->data, PLAYER_RIGHT_IDLE_TEXTURE_IDS.size(), true);
    for (auto& textureId : PLAYER_RIGHT_IDLE_TEXTURE_IDS)
    {
        rightIdleAnimation->Add(std::move(AnimationFrame{
            &ResourceManager::GetInstance().GetTexture(textureId),
            0.5f
            }));
    }
    rightIdleAnimation->Play();

    leftIdleAnimation = gameObject->AddComponent<AnimationController>(&renderer->data, PLAYER_LEFT_IDLE_TEXTURE_IDS.size(), true);
    for (auto& textureId : PLAYER_LEFT_IDLE_TEXTURE_IDS)
    {
        leftIdleAnimation->Add(std::move(AnimationFrame{
            &ResourceManager::GetInstance().GetTexture(textureId),
            0.5f
            }));
    }

    rightRunAnimation = gameObject->AddComponent<AnimationController>(&renderer->data, PLAYER_RIGHT_RUN_TEXTURE_IDS.size(), true);
    for (auto& textureId : PLAYER_RIGHT_RUN_TEXTURE_IDS)
    {
        rightRunAnimation->Add(std::move(AnimationFrame{
            &ResourceManager::GetInstance().GetTexture(textureId),
            0.25f
            }));
    }

    leftRunAnimation = gameObject->AddComponent<AnimationController>(&renderer->data, PLAYER_LEFT_RUN_TEXTURE_IDS.size(), true);
    for (auto& textureId : PLAYER_LEFT_RUN_TEXTURE_IDS)
    {
        leftRunAnimation->Add(std::move(AnimationFrame{
            &ResourceManager::GetInstance().GetTexture(textureId),
            0.25f
            }));
    }
}

void PlayerController::Destroy()
{
    UpdaterComponent::Destroy();
    BoxColliderComponent::Destroy();

    detector->eventhandler.Remove(&eventItem);
}

void PlayerController::Update()
{
    if (onGround && Game::GetKeyboardButtonStatus(GLFW_KEY_SPACE) == InputStatus::KEY_DOWN)
    {
        rigidbody->velocity.y += -400.0f;
        onGround = false;
#if USE_IRRKLANG_SOUND_ENGINE
        soundEngine->play2D("Assets/Sounds/jump.wav");
#endif
    }

    if (Game::GetKeyboardButtonStatus(GLFW_KEY_A) == InputStatus::KEY_HOLD)
    {
        rigidbody->velocity.x = -200.0f;
        RefreshAnimation(PlayerAnimatioState::LEFT_RUN);
    }
    else if (Game::GetKeyboardButtonStatus(GLFW_KEY_D) == InputStatus::KEY_HOLD)
    {
        rigidbody->velocity.x = 200.0f;
        RefreshAnimation(PlayerAnimatioState::RIGHT_RUN);
    }
    else
    {
        switch (currentState)
        {
        case PlayerAnimatioState::LEFT_RUN:
            RefreshAnimation(PlayerAnimatioState::LEFT_IDLE);
            break;
        case PlayerAnimatioState::RIGHT_RUN:
            RefreshAnimation(PlayerAnimatioState::RIGHT_IDLE);
            break;
        case PlayerAnimatioState::LEFT_IDLE:
        case PlayerAnimatioState::RIGHT_IDLE:
            break;
        }
        rigidbody->velocity.x = 0;
    }
}

void PlayerController::OnCollision(const Collision& collision)
{
    auto coin = collision.collidedObject->GetComponent<CoinController>();
    if (coin != nullptr)
    {
        ++coinNumber;
        coin->gameObject->isActive = false;
#if USE_IRRKLANG_SOUND_ENGINE
        soundEngine->play2D("Assets/Sounds/coin.wav");
#endif
        coinCollected.Invoke();
    }
}

void PlayerController::Inform()
{
    onGround = true;
}

void PlayerController::RefreshAnimation(PlayerAnimatioState newState)
{
    if (currentState == newState)
        return;

    switch (newState)
    {
    case PlayerAnimatioState::LEFT_RUN:
        rightIdleAnimation->Stop();
        leftIdleAnimation->Stop();
        rightRunAnimation->Stop();
        leftRunAnimation->Play(true);
        //It will use its mirror image with the same time
        if (currentState == PlayerAnimatioState::RIGHT_RUN)
        {
            leftRunAnimation->JumpToFrame(
                rightRunAnimation->GetCurrentIndex(),
                rightRunAnimation->GetCurrentTime()
            );
        }
        break;
    case PlayerAnimatioState::RIGHT_RUN:
        rightIdleAnimation->Stop();
        leftIdleAnimation->Stop();
        leftRunAnimation->Stop();
        rightRunAnimation->Play(true);
        //It will use its mirror image with the same time
        if (currentState == PlayerAnimatioState::LEFT_RUN)
        {
            rightRunAnimation->JumpToFrame(
                leftRunAnimation->GetCurrentIndex(),
                leftRunAnimation->GetCurrentTime()
            );
        }
        break;
    case PlayerAnimatioState::LEFT_IDLE:
        rightIdleAnimation->Stop();
        rightRunAnimation->Stop();
        leftRunAnimation->Stop();
        leftIdleAnimation->Play(true);
        break;
    case PlayerAnimatioState::RIGHT_IDLE:
        leftIdleAnimation->Stop();
        rightRunAnimation->Stop();
        leftRunAnimation->Stop();
        rightIdleAnimation->Play(true);
        break;
    }

    currentState = newState;
}