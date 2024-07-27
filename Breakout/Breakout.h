#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <irrklang/irrKlang.h>

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/UI/Text.h>
#include <Learning2DEngine/Physics/BoxCollider.h>
#include <Learning2DEngine/Physics/CircleCollider.h>

#include "Script/GameLevel.h"
#include "Script/BackgroundController.h"
#include "Script/PlayerController.h"
#include "Script/BallController.h"
#include "Script/PostProcessData.h"
#include "Script/PowerUpController.h"
#include "Script/PowerUpType.h"
#include "Script/CollisionResult.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Breakout : public virtual Learning2DEngine::System::Game
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

    Learning2DEngine::UI::Text  liveText;
    Learning2DEngine::UI::Text  startText;
    Learning2DEngine::UI::Text  levelSelectorText;
    Learning2DEngine::UI::Text  winText;
    Learning2DEngine::UI::Text  retryText;
protected:
    void InitSystem();
    void InitObjects();
    void Update() override;
    void Render() override;
    void LateRender() override;

    void ProcessInput();
    void ShakeScreen();
    void IsLiveLost();
    void IsLevelCompleted();

    void ResetLevel();
    void ResetPlayer();

    void SpawnPowerUps(glm::vec2 position);
    void ActivatePowerUp(PowerUpController& powerUp);
    bool IsPowerUpActive(const PowerUpType& type);
    void UpdatePowerUps();
    void ClearPowerUps();

    void DoCollisions();
    bool CheckCollision(const Learning2DEngine::Physics::BoxCollider& box1, const Learning2DEngine::Physics::BoxCollider& box2);
    CollisionResult CheckCollision(const Learning2DEngine::Physics::CircleCollider& ball, const Learning2DEngine::Physics::BoxCollider& box);
    Direction VectorDirection(glm::vec2 target);
    void CheckBricksCollision();
    void CheckPowerUpCollision();
    void CheckBallPlayerCollision();
public:
    Breakout();
    ~Breakout();

    void Init() override;
    void Terminate() override;
};