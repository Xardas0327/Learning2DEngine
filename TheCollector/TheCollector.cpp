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

    resourceManager.LoadTextureFromFile("Bushes", "Assets/Images/Bushes.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin", "Assets/Images/Coin.png", basicSettings);
    resourceManager.LoadTextureFromFile("Grounds", "Assets/Images/Grounds.png", basicSettings);
    resourceManager.LoadTextureFromFile("Knight", "Assets/Images/Knight.png", basicSettings);
    resourceManager.LoadTextureFromFile("Platforms", "Assets/Images/Platforms.png", basicSettings);

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