#include "TheCollector.h"

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/Physics/BoxColliderComponent.h>
#include <Learning2DEngine/Physics/CircleColliderComponent.h>

#include "GameController.h"

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Physics;

void TheCollector::Init()
{
    Game::Init();

    auto& renderManager = RenderManager::GetInstance();
    auto& resourceManager = ResourceManager::GetInstance();

    // Textures
    Texture2DSettings basicSettings;
    basicSettings.internalFormat = GL_RGBA;
    basicSettings.imageFormat = GL_RGBA;
    basicSettings.wrapS = GL_CLAMP_TO_EDGE;
    basicSettings.wrapT = GL_CLAMP_TO_EDGE;

    resourceManager.LoadTextureFromFile("SmallPlatform", "Assets/Images/Platforms/SmallPlatform.png", basicSettings);
    resourceManager.LoadTextureFromFile("LargePlatform", "Assets/Images/Platforms/LargePlatform.png", basicSettings);
    resourceManager.LoadTextureFromFile("Ground1", "Assets/Images/Grounds/Ground1.png", basicSettings);
    resourceManager.LoadTextureFromFile("Ground2", "Assets/Images/Grounds/Ground2.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush1", "Assets/Images/Bushes/Bush1.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush2", "Assets/Images/Bushes/Bush2.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush3", "Assets/Images/Bushes/Bush3.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush4", "Assets/Images/Bushes/Bush4.png", basicSettings);

    resourceManager.LoadTextureFromFile("Coin", "Assets/Images/Coins/Coin.png", basicSettings);

    resourceManager.LoadTextureFromFile("KnightRightIdle1", "Assets/Images/Knights/KnightRightIdle1.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightIdle2", "Assets/Images/Knights/KnightRightIdle2.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightIdle3", "Assets/Images/Knights/KnightRightIdle3.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightIdle4", "Assets/Images/Knights/KnightRightIdle4.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftIdle1", "Assets/Images/Knights/KnightLeftIdle1.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftIdle2", "Assets/Images/Knights/KnightLeftIdle2.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftIdle3", "Assets/Images/Knights/KnightLeftIdle3.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftIdle4", "Assets/Images/Knights/KnightLeftIdle4.png", basicSettings);

    resourceManager.LoadTextureFromFile("KnightRightRun1", "Assets/Images/Knights/KnightRightRun1.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun2", "Assets/Images/Knights/KnightRightRun2.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun3", "Assets/Images/Knights/KnightRightRun3.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun4", "Assets/Images/Knights/KnightRightRun4.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun5", "Assets/Images/Knights/KnightRightRun5.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun6", "Assets/Images/Knights/KnightRightRun6.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun7", "Assets/Images/Knights/KnightRightRun7.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun8", "Assets/Images/Knights/KnightRightRun8.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun9", "Assets/Images/Knights/KnightRightRun9.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun10", "Assets/Images/Knights/KnightRightRun10.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun11", "Assets/Images/Knights/KnightRightRun11.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun12", "Assets/Images/Knights/KnightRightRun12.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun13", "Assets/Images/Knights/KnightRightRun13.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun14", "Assets/Images/Knights/KnightRightRun14.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun15", "Assets/Images/Knights/KnightRightRun15.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightRun16", "Assets/Images/Knights/KnightRightRun16.png", basicSettings);

    resourceManager.LoadTextureFromFile("KnightLeftRun1", "Assets/Images/Knights/KnightLeftRun1.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun2", "Assets/Images/Knights/KnightLeftRun2.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun3", "Assets/Images/Knights/KnightLeftRun3.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun4", "Assets/Images/Knights/KnightLeftRun4.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun5", "Assets/Images/Knights/KnightLeftRun5.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun6", "Assets/Images/Knights/KnightLeftRun6.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun7", "Assets/Images/Knights/KnightLeftRun7.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun8", "Assets/Images/Knights/KnightLeftRun8.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun9", "Assets/Images/Knights/KnightLeftRun9.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun10", "Assets/Images/Knights/KnightLeftRun10.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun11", "Assets/Images/Knights/KnightLeftRun11.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun12", "Assets/Images/Knights/KnightLeftRun12.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun13", "Assets/Images/Knights/KnightLeftRun13.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun14", "Assets/Images/Knights/KnightLeftRun14.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun15", "Assets/Images/Knights/KnightLeftRun15.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftRun16", "Assets/Images/Knights/KnightLeftRun16.png", basicSettings);

    //Background color
    renderManager.SetClearColor(0.0f, 0.6f, 0.9f, 1.0f);

    // MSAA
    ActivateMSAA(4);

    // Camera
    Game::mainCamera.SetResolution(renderManager.GetResolution());

    //GameController
    auto gameController = GameObjectManager::GetInstance().CreateGameObject();
    gameController->AddComponent<GameController>();
}