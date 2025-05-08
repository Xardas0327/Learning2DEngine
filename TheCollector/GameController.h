#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>

#include "CoinController.h"
#include "PlayerController.h"

enum class GameStatus {Menu, Play, Ended};

class GameController : public virtual Learning2DEngine::System::UpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
protected:
    std::vector<CoinController*> coins;
    PlayerController* playerController;
    GameStatus gameStatus;

    GameController(Learning2DEngine::System::GameObject* gameObject);

    void Init() override;
    void Update() override;
    void CreateEnvironment();
    void StartPlay();
    void EndPlay();
};