#pragma once

#include <glm/glm.hpp>
#include <irrklang/irrKlang.h>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/System/LateUpdaterComponent.h>
#include <Learning2DEngine/UI/FontSizePair.h>
#include <Learning2DEngine/UI/SimpleText2DLateRenderComponent.h>

#include "GameLevel.h"
#include "BackgroundController.h"
#include "PlayerController.h"
#include "BallController.h"
#include "PostProcessData.h"
#include "PowerUpController.h"
#include "PowerUpType.h"
#include "IPowerUpActivation.h"
#include "PowerUpActivationEventItem.h"
#include "IBallHitPlayerAction.h"
#include "BallHitPlayerEventItem.h"
#include "IBallHitBrickAction.h"
#include "BallHitBrickEventItem.h"


enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class GameController final : public Learning2DEngine::System::UpdaterComponent,
                            public Learning2DEngine::System::LateUpdaterComponent,
                            public IPowerUpActivation,
                            public IBallHitPlayerAction,
                            public IBallHitBrickAction
{
    friend class Learning2DEngine::System::GameObject;
private:
    const Learning2DEngine::UI::FontSizePair fontSizePair;
    PostProcessData* postProcessData;
    irrklang::ISoundEngine* soundEngine;

    GameState state;
    std::vector<PowerUpController*> powerUps;
    std::vector<GameLevel> levels;
    unsigned int selectedLevel;
    unsigned int lifes;
    BackgroundController* backgroundController;
    PlayerController* playerController;
    BallController* ballController;
    float shakeTime;
    PowerUpActivationEventItem powerUpActivationEventItem;
    BallHitPlayerEventItem ballHitPlayerEventItem;
    BallHitBrickEventItem ballHitBrickEventItem;

    Learning2DEngine::UI::SimpleText2DLateRenderComponent* lifeText;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* startText;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* levelSelectorText;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* winText;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* retryText;


    GameController(Learning2DEngine::System::GameObject* gameObject, const Learning2DEngine::UI::FontSizePair& fontSizePair,
        PostProcessData* postProcessData);

    void ProcessInput();
    void ShakeScreen();
    void IsLifeLost();
    void IsLevelCompleted();

    void ResetLevel();
    void ResetPlayer();

    void SpawnPowerUps(glm::vec2 position);
    bool IsPowerUpActive(const PowerUpType& type);
    void UpdatePowerUps();
    void ClearPowerUps();

    void Init() override;
    void Destroy() override;
    void Update() override;
    void LateUpdate() override;

    void ActivatePowerUp(PowerUpType powerUpType) override;
    void BallHitPlayer() override;
    void BallHitBrick(BrickController* brick) override;
};

