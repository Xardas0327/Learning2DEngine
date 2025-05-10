#include "PlayerController.h"

#include <Learning2DEngine/DebugTool/Log.h>
#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/InputStatus.h>
#include <Learning2DEngine/System/ResourceManager.h>

#include "CoinController.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Physics;
using namespace irrklang;

PlayerController::PlayerController(GameObject* gameObject, ISoundEngine* soundEngine)
    : UpdaterComponent(gameObject), BaseUpdaterComponent(gameObject), Component(gameObject),
    BoxColliderComponent(gameObject, PLAYER_SIZE, ColliderType::DYNAMIC, ColliderMode::COLLIDER),
    BaseBoxColliderComponent(gameObject, PLAYER_SIZE, ColliderType::DYNAMIC, ColliderMode::COLLIDER),
    BaseColliderComponent(gameObject, ColliderType::DYNAMIC, ColliderMode::COLLIDER),
    onGround(false), rigidbody(nullptr), render(nullptr), coinNumber(0), detector(nullptr), eventItem(this),
    rightSide(ResourceManager::GetInstance().GetTexture(PLAYER_RIGHT_TEXTURE_ID)),
    leftSide(ResourceManager::GetInstance().GetTexture(PLAYER_LEFT_TEXTURE_ID)),
    coinCollected(), soundEngine(soundEngine)
{
}

void PlayerController::Init()
{
    UpdaterComponent::Init();
    BoxColliderComponent::Init();

    gameObject->transform.SetScale(PLAYER_SIZE);

    render = gameObject->AddComponent<SpriteRenderComponent>();
    render->data.texture = &rightSide;
    rigidbody = gameObject->AddComponent<Rigidbody>(glm::vec2(0.0f, 0.0f), true);
    rigidbody->gravityMultiplier = 50.0f;
    InitRigidbody();

    detector = gameObject->AddComponent<PlatformDetectorController>(glm::vec2(30.0f, 5.0f), glm::vec2(10.0f, 45.0f));
    detector->eventhandler.Add(&eventItem);
}

void PlayerController::Destroy()
{
    UpdaterComponent::Destroy();
    BoxColliderComponent::Destroy();

    render->data.ClearTexture();
    detector->eventhandler.Remove(&eventItem);
}

void PlayerController::Update()
{
    if (onGround && Game::GetKeyboardButtonStatus(GLFW_KEY_SPACE) == InputStatus::KEY_DOWN)
    {
        rigidbody->velocity.y += -400.0f;
        onGround = false;
        soundEngine->play2D("Assets/Sounds/jump.wav");
    }

    if (Game::GetKeyboardButtonStatus(GLFW_KEY_A) == InputStatus::KEY_HOLD)
    {
        rigidbody->velocity.x = -200.0f;
        render->data.texture = &leftSide;
    }
    else if (Game::GetKeyboardButtonStatus(GLFW_KEY_D) == InputStatus::KEY_HOLD)
    {
        rigidbody->velocity.x = 200.0f;
        render->data.texture = &rightSide;
    }
    else
    {
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
        soundEngine->play2D("Assets/Sounds/coin.wav");
        coinCollected.Invoke();
    }
}

void  PlayerController::Inform()
{
    onGround = true;
}