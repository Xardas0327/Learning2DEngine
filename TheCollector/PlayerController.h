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

const std::string PLAYER_TEXTURE_ID = "Knight";
const int PLAYER_IDLE_ANIMATION_NUMBER = 4;
const int PLAYER_RUN_ANIMATION_NUMBER = 16;
const glm::mat4x2 PLAYER_ANIMATION_UV_OFFSET = glm::mat4x2{
        0.03125f, 0.0703125f,
        -0.03125f, 0.0703125f,
        -0.03125f, -0.03125f,
        0.03125f, -0.03125f,
};

class PlayerController : public Learning2DEngine::System::UpdaterComponent,
                        public Learning2DEngine::Physics::BoxColliderComponent,
                        public IDetectorCustomer
{
    friend class Learning2DEngine::System::GameObject;
protected:
    enum class PlayerAnimatioState {
        LEFT_IDLE,
        RIGHT_IDLE,
        LEFT_RUN,
        RIGHT_RUN
    };

    bool onGround;
    PlatformDetectorController* detector;
    DetectorEventItem eventItem;
    Learning2DEngine::Animator::AnimationController* rightIdleAnimation;
    Learning2DEngine::Animator::AnimationController* leftIdleAnimation;
    Learning2DEngine::Animator::AnimationController* rightRunAnimation;
    Learning2DEngine::Animator::AnimationController* leftRunAnimation;
    PlayerAnimatioState currentState;
    irrklang::ISoundEngine* soundEngine;

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