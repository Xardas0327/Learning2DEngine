#include "Snake.h"

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/Render/SpriteRenderer.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Random.h>
#include <Learning2DEngine/UI/Text2DRenderer.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;

Snake::Snake()
    : levelResolution(10, 10), fontSizePair("Assets/Fonts/OCRAEXT.TTF", 24), startMoveWaitingTime(0.5f), dencreaseTimeAfterFood(0.005f), 
    baseSnakeLength(3),
    state(GameState::GAME_MENU), score(0),  unitSize(0), food(nullptr), player(),
    moveWaitingTime(0.0f), actualWaitingTime(0.0f), moveDirection(), lastMoveDirection(), scoreText(), startText()
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

    ResetLevel();
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

    // Unit
    unitSize = glm::vec2(resolution.GetWidth() / levelResolution.x, resolution.GetHeight() / levelResolution.y);

    //Food
    food = new GameObject(
        Transform(
            glm::vec2(0.0f, 0.0f),
            unitSize
        )
    );
    food->AddComponent<SpriteRenderer, const Texture2D&, glm::vec3>(
        ResourceManager::GetInstance().GetTexture("Unit"),
        glm::vec3(0.75f, 0.0f, 0.0f)
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
    for (std::list<GameObject*>::iterator it = player.begin(); it != player.end(); ++it) {
        GameObject::Destroy(*it);
    }
    player.clear();
    GameObject::Destroy(food);

	Text2DRenderer::GetInstance().Terminate();
	Game::Terminate();
}

void Snake::Update()
{
    ProcessInput();

    MoveSnake();
}

void Snake::Render()
{
    food->GetComponent<SpriteRenderer>()->Draw();

    for (std::list<GameObject*>::iterator it = player.begin(); it != player.end(); ++it) {
        (*it)->GetComponent<SpriteRenderer>()->Draw();
    }
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
        if (lastMoveDirection == Direction::LEFT || lastMoveDirection == Direction::RIGHT)
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
        else if (lastMoveDirection == Direction::UP || lastMoveDirection == Direction::DOWN)
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

    auto& resourceManager = ResourceManager::GetInstance();
    for (std::list<GameObject*>::iterator it = player.begin(); it != player.end(); ++it) {
        GameObject::Destroy(*it);
    }
    player.clear();
    for (unsigned int i = 0; i < baseSnakeLength; ++i)
    {
        player.push_front(CreateNewPlayerUnit(glm::vec2(i * unitSize.x, 0.0f)));
    }

    score = 0;
    RefreshScore();
    moveWaitingTime = startMoveWaitingTime;
    actualWaitingTime = startMoveWaitingTime;
    moveDirection = Direction::RIGHT;
    lastMoveDirection = Direction::RIGHT;
    GenerateNextFood();
}

void Snake::MoveSnake()
{
    if (state != GameState::GAME_ACTIVE)
        return;

    actualWaitingTime -= Game::GetDeltaTime();
    if (actualWaitingTime < 0)
    {
        glm::vec2 moveVector = glm::vec2(0.0f);
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
        }

        lastMoveDirection = moveDirection;
        glm::vec2 newPostion = player.front()->transform.position + glm::vec2(moveVector.x * unitSize.x, moveVector.y * unitSize.y);

        if (IsOut(newPostion) || IsInSnake(newPostion))
        {
            state = GameState::GAME_MENU;
        }
        else
        {
            if (newPostion == food->transform.position)
            {
                player.push_front(CreateNewPlayerUnit(newPostion));
                EatFood();
                moveWaitingTime -= dencreaseTimeAfterFood;
                if (moveWaitingTime < 0)
                    moveWaitingTime = 0;
            }
            else
            {
                auto end = player.rbegin();
                auto beforeIt = player.rbegin();
                ++beforeIt;
                while (beforeIt != player.rend())
                {
                    (*end)->transform.position = (*beforeIt)->transform.position;
                    ++end;
                    ++beforeIt;
                }
                player.front()->transform.position = newPostion;
            }


            actualWaitingTime += moveWaitingTime;
            if (actualWaitingTime < 0)
                actualWaitingTime = 0;
        }
    }
}

bool Snake::IsOut(const glm::vec2 position)
{
    auto cameraResolution = Game::GetCameraResolution();

    return position.x < 0 || position.x >= cameraResolution.GetWidth()
        || position.y < 0 || position.y >= cameraResolution.GetHeight();
}

bool Snake::IsInSnake(const glm::vec2 position)
{
    for (std::list<GameObject*>::iterator it = player.begin(); it != player.end(); ++it) {
        if ((*it)->transform.position == position)
        {
            return true;
        }
    }
    return false;
}

void Snake::GenerateNextFood()
{
    if (player.size() >= levelResolution.x * levelResolution.y)
    {
        state = GameState::GAME_MENU;
        return;
    }

    while (true)
    {
        int x = Random::GetNumber(0, levelResolution.x);
        int y = Random::GetNumber(0, levelResolution.y);
        food->transform.position = glm::vec2(x * unitSize.x, y * unitSize.y);

        if (!IsInSnake(food->transform.position))
            break;
    }
}

void Snake::EatFood()
{
    ++score;
    RefreshScore();
    GenerateNextFood();
}

void Snake::RefreshScore()
{
    scoreText.text = "Score: " + std::to_string(score);
}

GameObject* Snake::CreateNewPlayerUnit(const glm::vec2 position)
{
    auto playerUnit = new GameObject(
        Transform(
            position,
            unitSize
        )
    );
    playerUnit->AddComponent<SpriteRenderer, const Texture2D&, glm::vec3>(
        ResourceManager::GetInstance().GetTexture("Unit"),
        glm::vec3(0.0f, 0.75f, 0.0f)
    );

    return playerUnit;
}