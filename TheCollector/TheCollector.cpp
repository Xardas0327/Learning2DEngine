#include "TheCollector.h"

#include <Learning2DEngine/System/GameObjectManager.h>

#include "GameController.h"

using namespace Learning2DEngine::System;

void TheCollector::Init()
{
    // MSAA
    ActivateMSAA(4);

    //GameController
    auto gameController = GameObjectManager::GetInstance().CreateGameObject();
    gameController->AddComponent<GameController>();
}