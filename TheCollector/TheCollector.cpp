#include "TheCollector.h"

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>
#include <Learning2DEngine/System/GameObject.h>
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

    resourceManager.LoadTextureFromFile("Coin1", "Assets/Images/Coins/Coin1.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin2", "Assets/Images/Coins/Coin2.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin3", "Assets/Images/Coins/Coin3.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin4", "Assets/Images/Coins/Coin4.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin5", "Assets/Images/Coins/Coin5.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin6", "Assets/Images/Coins/Coin6.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin7", "Assets/Images/Coins/Coin7.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin8", "Assets/Images/Coins/Coin8.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin9", "Assets/Images/Coins/Coin9.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin10", "Assets/Images/Coins/Coin10.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin11", "Assets/Images/Coins/Coin11.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin12", "Assets/Images/Coins/Coin12.png", basicSettings);

    resourceManager.LoadTextureFromFile("KnightRightIdle1", "Assets/Images/Knights/KnightRightIdle1.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightIdle2", "Assets/Images/Knights/KnightRightIdle2.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightIdle3", "Assets/Images/Knights/KnightRightIdle3.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRightIdle4", "Assets/Images/Knights/KnightRightIdle4.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftIdle1", "Assets/Images/Knights/KnightLeftIdle1.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftIdle2", "Assets/Images/Knights/KnightLeftIdle2.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftIdle3", "Assets/Images/Knights/KnightLeftIdle3.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeftIdle4", "Assets/Images/Knights/KnightLeftIdle4.png", basicSettings);

    //Background color
    renderManager.SetClearColor(0.0f, 0.6f, 0.9f, 1.0f);

    // MSAA
    ActivateMSAA(4);

    // Camera
    Game::mainCamera.SetResolution(renderManager.GetResolution());

    //GameController
    auto gameController = GameObject::Create();
    gameController->AddComponent<GameController>();
}