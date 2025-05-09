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

    resourceManager.LoadTextureFromFile("SmallPlatform", "Assets/Images/SmallPlatform.png", basicSettings);
    resourceManager.LoadTextureFromFile("LargePlatform", "Assets/Images/LargePlatform.png", basicSettings);
    resourceManager.LoadTextureFromFile("Coin", "Assets/Images/Coin.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightRight", "Assets/Images/KnightRight.png", basicSettings);
    resourceManager.LoadTextureFromFile("KnightLeft", "Assets/Images/KnightLeft.png", basicSettings);
    resourceManager.LoadTextureFromFile("Ground1", "Assets/Images/Ground1.png", basicSettings);
    resourceManager.LoadTextureFromFile("Ground2", "Assets/Images/Ground2.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush1", "Assets/Images/Bush1.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush2", "Assets/Images/Bush2.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush3", "Assets/Images/Bush3.png", basicSettings);
    resourceManager.LoadTextureFromFile("Bush4", "Assets/Images/Bush4.png", basicSettings);

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