#include "Snake.h"

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/UI/TextCharacterSet.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;

Snake::Snake()
    : fontSizePair("Assets/Fonts/arial.ttf", 24), gameController(nullptr)
{

}

Snake::~Snake()
{

}

void Snake::Init()
{
	Game::Init();

    auto& resourceManager = ResourceManager::GetInstance();

    // Textures
    Texture2DSettings alphaSettings;
    alphaSettings.internalFormat = GL_RGBA;
    alphaSettings.imageFormat = GL_RGBA;

    resourceManager.LoadTextureFromFile("Unit", "Assets/Images/Unit.png", alphaSettings);

    // Text
    TextCharacterSet::GetInstance().Load(fontSizePair);

    // MSAA
    ActivateMSAA(4);

    // Camera
    Game::mainCamera.SetResolution(RenderManager::GetInstance().GetResolution());

    //GameController
    auto gameControllerObject = GameObject::Create();
    gameController = gameControllerObject->AddComponent<GameController, const FontSizePair&>(fontSizePair);
}

void Snake::Terminate()
{
    TextCharacterSet::GetInstance().Clear();
	Game::Terminate();
}