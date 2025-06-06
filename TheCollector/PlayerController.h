#pragma once

#include "Macro.h"

#include <glm/glm.hpp>
#if USE_IRRKLANG_SOUND_ENGINE
#include <irrklang/irrKlang.h>
#endif
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
const std::vector<std::string> PLAYER_RIGHT_RUN_TEXTURE_IDS = {
    "KnightRightRun1", "KnightRightRun2", "KnightRightRun3", "KnightRightRun4",
    "KnightRightRun5", "KnightRightRun6", "KnightRightRun7", "KnightRightRun8",
    "KnightRightRun9", "KnightRightRun10", "KnightRightRun11", "KnightRightRun12",
    "KnightRightRun13", "KnightRightRun14", "KnightRightRun15", "KnightRightRun16"
};
const std::vector<std::string> PLAYER_LEFT_RUN_TEXTURE_IDS = {
    "KnightLeftRun1", "KnightLeftRun2", "KnightLeftRun3", "KnightLeftRun4",
    "KnightLeftRun5", "KnightLeftRun6", "KnightLeftRun7", "KnightLeftRun8",
    "KnightLeftRun9", "KnightLeftRun10", "KnightLeftRun11", "KnightLeftRun12",
    "KnightLeftRun13", "KnightLeftRun14", "KnightLeftRun15", "KnightLeftRun16"
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
#if USE_IRRKLANG_SOUND_ENGINE
    irrklang::ISoundEngine* soundEngine;
#endif

    PlayerController(Learning2DEngine::System::GameObject* gameObject
#if USE_IRRKLANG_SOUND_ENGINE
        , irrklang::ISoundEngine* soundEngine
#endif
    );

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