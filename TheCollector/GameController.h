#pragma once

#include "Macro.h"

#include <glm/glm.hpp>
#include <vector>
#if USE_IRRKLANG_SOUND_ENGINE
#include <irrklang/irrKlang.h>
#endif

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/UI/SimpleText2DRenderComponent.h>

#include "CoinController.h"
#include "PlayerController.h"
#include "DetectorEventItem.h"
#include "MovingPlatformController.h"

enum class GameStatus {Menu, Play, Ended};

#define PLAY_TIME 45.0f

class GameController : public Learning2DEngine::System::UpdaterComponent,
                        public IDetectorCustomer
{
    friend class Learning2DEngine::System::GameObject;
protected:
    std::vector<CoinController*> coins;
    std::vector<MovingPlatformController*> movingPlatforms;
    PlayerController* playerController;
    GameStatus gameStatus;
    const Learning2DEngine::UI::FontSizePair fontSizePair;
    DetectorEventItem playerCoinEventItem;
    float currentPlayTime;
    Learning2DEngine::UI::SimpleText2DRenderComponent* scoreText;
    Learning2DEngine::UI::SimpleText2DRenderComponent* playTimeText;
    Learning2DEngine::UI::SimpleText2DRenderComponent* descriptionText;
    Learning2DEngine::UI::SimpleText2DRenderComponent* startText;
    Learning2DEngine::UI::SimpleText2DRenderComponent* winText;
    Learning2DEngine::UI::SimpleText2DRenderComponent* loseText;
    Learning2DEngine::UI::SimpleText2DRenderComponent* endText;
#if USE_IRRKLANG_SOUND_ENGINE
    irrklang::ISoundEngine* soundEngine;
#endif

    GameController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Destroy() override;
    void Update() override;
    void InitEnvironment();
    void InitTexts();
    void ShowMenu();
    void StartPlay();
    void EndPlay();
    void RefreshScoreText();
    void RefreshPlayTimeText();
    void Inform() override;
};