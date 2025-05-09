#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/UI/SimpleText2DLateRenderComponent.h>

#include "CoinController.h"
#include "PlayerController.h"
#include "DetectorEventItem.h"

enum class GameStatus {Menu, Play, Ended};

#define PLAY_TIME 45.0f

class GameController : public virtual Learning2DEngine::System::UpdaterComponent,
                        public IDetectorCustomer
{
    friend class Learning2DEngine::System::GameObject;
protected:
    std::vector<CoinController*> coins;
    PlayerController* playerController;
    GameStatus gameStatus;
    const Learning2DEngine::UI::FontSizePair fontSizePair;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* scoreText;
    Learning2DEngine::UI::SimpleText2DLateRenderComponent* playTimeText;
    DetectorEventItem playerCoinEventItem;
    float currentPlayTime;

    GameController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Destroy() override;
    void Update() override;
    void CreateEnvironment();
    void StartPlay();
    void EndPlay();
    void RefreshScoreText();
    void RefreshPlayTimeText();
    void Inform() override;
};