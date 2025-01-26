#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <irrklang/irrKlang.h>

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/UI/Text2DLateRenderer.h>
#include <Learning2DEngine/Physics/BoxCollider.h>
#include <Learning2DEngine/Physics/CircleCollider.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>

#include "Script/GameLevel.h"
#include "Script/BackgroundController.h"
#include "Script/PlayerController.h"
#include "Script/BallController.h"
#include "Script/PostProcessData.h"
#include "Script/PowerUpController.h"
#include "Script/PowerUpType.h"
#include "Script/PowerUpActivation.h"
#include "Script/PowerUpActivationEventItem.h"
#include "Script/CollisionResult.h"
#include "Script/BallHitPlayerAction.h"
#include "Script/BallHitPlayerEventItem.h"
#include "Script/BallHitBrickAction.h"
#include "Script/BallHitBrickEventItem.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Breakout : public virtual Learning2DEngine::System::Game,
                 public virtual PowerUpActivation,
                 public virtual BallHitPlayerAction,
                 public virtual BallHitBrickAction
{
private:
    GameState state;
    std::vector<PowerUpController*> powerUps;
    std::vector<GameLevel> levels;
    unsigned int selectedLevel;
    unsigned int lives;
    BackgroundController* backgroundController;
    PlayerController* playerController;
    BallController* ballController;
    irrklang::ISoundEngine* soundEngine;
    const Learning2DEngine::UI::FontSizePair fontSizePair;
    PostProcessData* postProcessData;
    float shakeTime;
    PowerUpActivationEventItem powerUpActivationEventItem;
    BallHitPlayerEventItem ballHitPlayerEventItem;
    BallHitBrickEventItem ballHitBrickEventItem;

    Learning2DEngine::UI::Text2DLateRenderer* liveText;
    Learning2DEngine::UI::Text2DLateRenderer* startText;
    Learning2DEngine::UI::Text2DLateRenderer* levelSelectorText;
    Learning2DEngine::UI::Text2DLateRenderer* winText;
    Learning2DEngine::UI::Text2DLateRenderer* retryText;

protected:
    void InitSystem();
    void InitObjects();
    void Update() override;

    void ProcessInput();
    void ShakeScreen();
    void IsLiveLost();
    void IsLevelCompleted();

    void ResetLevel();
    void ResetPlayer();

    void SpawnPowerUps(glm::vec2 position);
    bool IsPowerUpActive(const PowerUpType& type);
    void UpdatePowerUps();
    void ClearPowerUps();

    void DoCollisions();
    bool CheckCollision(const Learning2DEngine::Physics::BoxCollider& box1, const Learning2DEngine::Physics::BoxCollider& box2);
    bool CheckCollision(const Learning2DEngine::Physics::BoxCollider& box1, const Learning2DEngine::Physics::BoxColliderComponent& box2);
    CollisionResult CheckCollision(const Learning2DEngine::Physics::CircleCollider& ball, const Learning2DEngine::Physics::BoxCollider& box);
    CollisionResult CheckCollision(const Learning2DEngine::Physics::CircleCollider& ball, const Learning2DEngine::Physics::BoxColliderComponent& box);
    CollisionResult CheckCollision(const Learning2DEngine::Physics::CircleColliderComponent& ball, const Learning2DEngine::Physics::BoxColliderComponent& box);
    OldDirection VectorDirection(glm::vec2 target);
public:
    Breakout();
    ~Breakout();

    void Init() override;
    void Terminate() override;
    void ActivatePowerUp(PowerUpType powerUp) override;
    void BallHitPlayer() override;
    void BallHitBrick(BrickController* brick) override;
};