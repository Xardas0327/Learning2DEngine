#include "Breakout.h"

#include <string>

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/UI/TextCharacterSet.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;
using namespace Learning2DEngine::Physics;

Breakout::Breakout()
    : fontSizePair("Assets/Fonts/OCRAEXT.TTF", 24), postProcessData(nullptr), gameController(nullptr)
{

}

Breakout::~Breakout()
{
}

void Breakout::Init()
{
    Game::Init();

    auto& resourceManager = ResourceManager::GetInstance();

    // Shaders
    resourceManager.LoadShaderFromFile(std::string("PostProcessing"), "Assets/Shaders/PostProcessing.vs", "Assets/Shaders/PostProcessing.fs");

    // Textures
    Texture2DSettings alphaSettings;
    alphaSettings.internalFormat = GL_RGBA;
    alphaSettings.imageFormat = GL_RGBA;
    Texture2DSettings nonAlphaSettings;

    resourceManager.LoadTextureFromFile("background", "Assets/Images/background.jpg", nonAlphaSettings);
    resourceManager.LoadTextureFromFile("face", "Assets/Images/awesomeface.png", alphaSettings);
    resourceManager.LoadTextureFromFile("block", "Assets/Images/block.png", nonAlphaSettings);
    resourceManager.LoadTextureFromFile("block_solid", "Assets/Images/block_solid.png", nonAlphaSettings);
    resourceManager.LoadTextureFromFile("paddle", "Assets/Images/paddle.png", alphaSettings);
    resourceManager.LoadTextureFromFile("particle", "Assets/Images/particle.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_speed", "Assets/Images/powerup_speed.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_sticky", "Assets/Images/powerup_sticky.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_increase", "Assets/Images/powerup_increase.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_confuse", "Assets/Images/powerup_confuse.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_chaos", "Assets/Images/powerup_chaos.png", alphaSettings);
    resourceManager.LoadTextureFromFile("powerup_passthrough", "Assets/Images/powerup_passthrough.png", alphaSettings);

    // Text
    TextCharacterSet::GetInstance().Load(fontSizePair);

    // MSAA
    ActivateMSAA(4);

    // PostProcessEffect
    postProcessData = new PostProcessData(resourceManager.GetShader("PostProcessing"));
    ActivatePostProcessEffect();
    UsePostProcessEffect(postProcessData->shader);

    // Camera
    mainCamera.SetResolution(RenderManager::GetInstance().GetResolution());

    // GameController
    auto gameControllerObject = GameObject::Create();
    gameController = gameControllerObject->AddComponent<
        GameController,
        const FontSizePair&,
        PostProcessData*
    >(fontSizePair, postProcessData);
}

void Breakout::Terminate()
{
    delete postProcessData;

    TextCharacterSet::GetInstance().Clear();
    Game::Terminate();
}