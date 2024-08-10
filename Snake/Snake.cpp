#include "Snake.h"

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/UI/Text2DRenderer.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;

Snake::Snake()
	: state(GameState::GAME_MENU), score(0), levelResolution(10), unitSize(0),
    fontSizePair("Assets/Fonts/OCRAEXT.TTF", 24), player(nullptr), 
    waitingTime(0.0f), startWaitingTime(0.75f), moveDirection(), scoreText(), startText()
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
    auto& resourceManager = ResourceManager::GetInstance();

    // Textures
    Texture2DSettings alphaSettings;
    alphaSettings.internalFormat = GL_RGBA;
    alphaSettings.imageFormat = GL_RGBA;

    resourceManager.LoadTextureFromFile("Unit", "Assets/Images/Unit.png", alphaSettings);

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

    //Unit
    unitSize = resolution.GetHeight() < resolution.GetWidth() ? resolution.GetHeight() / levelResolution : resolution.GetWidth() / levelResolution;

    //Player
    player = new GameObject(
        Transform(
            glm::vec2(0.0f, 0.0f),
            glm::vec2(unitSize, unitSize)
        )
    );
    player->AddComponent<SpriteRenderer, const Texture2D&, glm::vec3>(
        ResourceManager::GetInstance().GetTexture("Unit"),
        glm::vec3(0.0f, 0.75f, 0.0f)
    );

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
    GameObject::Destroy(player);

	Text2DRenderer::GetInstance().Terminate();
	Game::Terminate();
}

void Snake::Update()
{
    ProcessInput();

    if (state != GAME_ACTIVE)
        return;

    waitingTime -= Game::GetDeltaTime();
    if (waitingTime < 0)
    {
        waitingTime += startWaitingTime;
        glm::vec2 moveVector;
        switch (moveDirection)
        {
        case Direction::UP:
            moveVector = VECTOR_UP;
            break;
        case Direction::DOWN:
            moveVector = VECTOR_DOWN;
            break;
        case Direction::RIGHT:
            moveVector = VECTOR_RIGHT;
            break;
        case Direction::LEFT:
            moveVector = VECTOR_LEFT;
            break;
        default:
            break;
        }

        player->transform.position += glm::vec2(moveVector.x * unitSize, moveVector.y * unitSize);
    }
}

void Snake::Render()
{
    player->GetComponent<SpriteRenderer>()->Draw();
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

    switch (state)
    {
    case GameState::GAME_MENU:
        if (Game::inputKeys[GLFW_KEY_ENTER] == InputStatus::KEY_DOWN)
        {
            state = GameState::GAME_ACTIVE;
            ResetLevel();
        }
    case GameState::GAME_ACTIVE:
        if (moveDirection == Direction::LEFT || moveDirection == Direction::RIGHT)
        {
            if (Game::inputKeys[GLFW_KEY_W] == InputStatus::KEY_DOWN
                || Game::inputKeys[GLFW_KEY_UP] == InputStatus::KEY_DOWN)
            {
                moveDirection = Direction::UP;
            }
            else if (Game::inputKeys[GLFW_KEY_S] == InputStatus::KEY_DOWN
                || Game::inputKeys[GLFW_KEY_DOWN] == InputStatus::KEY_DOWN)
            {
                moveDirection = Direction::DOWN;
            }
        }
        else if (moveDirection == Direction::UP || moveDirection == Direction::DOWN)
        {
            if (Game::inputKeys[GLFW_KEY_A] == InputStatus::KEY_DOWN
                || Game::inputKeys[GLFW_KEY_LEFT] == InputStatus::KEY_DOWN)
            {
                moveDirection = Direction::LEFT;
            }
            else if (Game::inputKeys[GLFW_KEY_D] == InputStatus::KEY_DOWN
                || Game::inputKeys[GLFW_KEY_RIGHT] == InputStatus::KEY_DOWN)
            {
                moveDirection = Direction::RIGHT;
            }
        }
        break;
    }
}

void Snake::ResetLevel()
{
    score = 0;
    scoreText.text = "Score: " + std::to_string(score);
    waitingTime = startWaitingTime;
    moveDirection = Direction::RIGHT;
}