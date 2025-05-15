#pragma once

#include <glm/glm.hpp>
#include <irrklang/irrKlang.h>
#include <vector>

#include <Learning2DEngine/Physics/Rigidbody.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/EventSystem/EventHandler.h>
#include <Learning2DEngine/Animator/AnimationController.h>

#include "PlatformDetectorController.h"
#include "DetectorEventItem.h"

const glm::vec2 PLAYER_SIZE(50.0f, 50.0f);
const glm::vec2 PLAYER_START_POSITION(200.0f, 450.0f);

const std::vector<std::string> PLAYER_RIGHT_IDLE_TEXTURE_IDS = {
    "KnightRightIdle1", "KnightRightIdle2", "KnightRightIdle3", "KnightRightIdle4"
};
const std::vector<std::string> PLAYER_LEFT_IDLE_TEXTURE_IDS = {
    "KnightLeftIdle1", "KnightLeftIdle2", "KnightLeftIdle3", "KnightLeftIdle4"
};

class PlayerController : public virtual Learning2DEngine::System::UpdaterComponent,
                        public virtual Learning2DEngine::Physics::BoxColliderComponent,
                        public IDetectorCustomer
{
    friend class Learning2DEngine::System::GameObject;
protected:
    enum class PlayerAnimatioState {
        IDLE_LEFT,
        IDLE_RIGHT
    };

    bool onGround;
    PlatformDetectorController* detector;
    irrklang::ISoundEngine* soundEngine;
    DetectorEventItem eventItem;
    Learning2DEngine::Animator::AnimationController* rightIdleAnimation;
    Learning2DEngine::Animator::AnimationController* leftIdleAnimation;
    PlayerAnimatioState currentState;

    PlayerController(Learning2DEngine::System::GameObject* gameObject, irrklang::ISoundEngine* soundEngine);

    void Init() override;
    void Destroy() override;
    void Update() override;
    void OnCollision(const Learning2DEngine::Physics::Collision& collision) override;
    void Inform() override;
    void RefreshAnimation(PlayerAnimatioState newState);
public:
    Learning2DEngine::Physics::Rigidbody* rigidbody;
    int coinNumber;
    Learning2DEngine::EventSystem::EventHandler<> coinCollected;
};