#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <irrklang/irrKlang.h>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/UI/SimpleText2DLateRenderComponent.h>

#include "CoinController.h"
#include "PlayerController.h"
#include "DetectorEventItem.h"
#include "MovingPlatformController.h"

enum class GameStatus {Menu, Play, Ended};

#define PLAY_TIME 45.0f

class GameController : public virtual Learning2DEngine::System::UpdaterComponent,
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
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* scoreText;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* playTimeText;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* description1Text;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* description2Text;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* startText;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* winText;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* loseText;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* endText;
    irrklang::ISoundEngine* soundEngine;

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