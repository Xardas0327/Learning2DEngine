#include "Snake.h"

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/UI/Text2DRenderer.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;

Snake::Snake()
	: state(GameState::GAME_MENU), score(0), fontSizePair("Assets/Fonts/OCRAEXT.TTF", 24),
	scoreText(), startText()
{

}

Snake::~Snake()
{

}

void Snake::Init()
{
	Game::Init();

	InitSystem();
	InitObjects();
}

void Snake::InitSystem()
{
    // Text
    auto& textRenderer = Text2DRenderer::GetInstance();
    textRenderer.Init(RenderManager::GetInstance().GetResolution());
    textRenderer.Load(fontSizePair);

    // MSAA
    ActivateMSAA(4);
}

void Snake::InitObjects()
{
    const Resolution resolution = RenderManager::GetInstance().GetResolution();
    // Camera
    SetCameraResolution(resolution);

    // Text
    scoreText = {
        fontSizePair,
        "Score: " + std::to_string(score),
        5.0f,
        5.0f
    };
    startText = {
        fontSizePair,
        "Press ENTER to start",
        250.0f,
        static_cast<float>(resolution.GetHeight() / 2)
    };

}

void Snake::Terminate()
{
	Text2DRenderer::GetInstance().Terminate();
	Game::Terminate();
}

void Snake::Update()
{
    ProcessInput();
}

void Snake::Render()
{

}

void Snake::LateRender()
{
    auto& textRenderer = Text2DRenderer::GetInstance();
    textRenderer.RenderText(scoreText);

    if (state == GameState::GAME_MENU)
    {
        textRenderer.RenderText(startText);
    }
}

void Snake::ProcessInput()
{
    if (Game::inputKeys[GLFW_KEY_ESCAPE] == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }
}

void Snake::ResetLevel()
{

}