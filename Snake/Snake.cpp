#include "Snake.h"

#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Random.h>
#include <Learning2DEngine/UI/Text2DRenderer.h>
#include <Learning2DEngine/UI/TextCharacterSet.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::UI;

Snake::Snake()
    : levelResolution(MAP_SIZE), fontSizePair("Assets/Fonts/arial.ttf", 24), startMoveWaitingTime(START_MOVE_WAITING_TIME),
    dencreaseTimeAfterEat(TIME_DECREASE), baseSnakeLength(3),
    state(GameState::GAME_MENU), score(0),  unitSize(0), foodController(nullptr), playerController(nullptr),
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
    TextCharacterSet::GetInstance().Load(fontSizePair);
    Text2DRenderer::GetInstance().Init(RenderManager::GetInstance().GetResolution());

    // MSAA
    ActivateMSAA(4);
}

void Snake::InitObjects()
{
    const Resolution resolution = RenderManager::GetInstance().GetResolution();

    // Camera
    mainCamera.SetResolution(resolution);

    // Unit
    unitSize = glm::vec2(resolution.GetWidth() / levelResolution.x, resolution.GetHeight() / levelResolution.y);

    //Player
    auto player = new GameObject();
    playerController = player->AddComponent<PlayerController, const std::string&>("Unit");

    //Food
    auto food = new GameObject(
        Transform(
            glm::vec2(0.0f, 0.0f),
            unitSize
        )
    );
    foodController = food->AddComponent<FoodController, const std::string&>("Unit");

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
        175.0f,
        static_cast<float>(resolution.GetHeight() / 2)
    };

}

void Snake::Terminate()
{
    GameObject::Destroy(playerController);
    GameObject::Destroy(foodController);

	Text2DRenderer::GetInstance().Terminate();
    TextCharacterSet::GetInstance().Clear();
	Game::Terminate();
}

void Snake::Update()
{
    ProcessInput();

    MoveSnake();
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
    playerController->Regenerate(unitSize);

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
        glm::vec2 newPostion = playerController->GetHeadPosition() + glm::vec2(moveVector.x * unitSize.x, moveVector.y * unitSize.y);

        if (IsOut(newPostion) || playerController->IsInSnake(newPostion))
        {
            state = GameState::GAME_MENU;
        }
        else
        {
            if (newPostion == foodController->gameObject->transform.position)
            {
                playerController->IncreaseSize(newPostion, unitSize);
                EatFood();
                moveWaitingTime -= dencreaseTimeAfterEat;
                if (moveWaitingTime < 0)
                    moveWaitingTime = 0;
            }
            else
            {
                playerController->Move(newPostion);
            }


            actualWaitingTime += moveWaitingTime;
            if (actualWaitingTime < 0)
                actualWaitingTime = 0;
        }
    }
}

bool Snake::IsOut(const glm::vec2 position)
{
    auto cameraResolution = Game::mainCamera.GetResolution();

    return position.x < 0 || position.x >= cameraResolution.GetWidth()
        || position.y < 0 || position.y >= cameraResolution.GetHeight();
}

void Snake::GenerateNextFood()
{
    if (playerController->GetSize() >= levelResolution.x * levelResolution.y)
    {
        state = GameState::GAME_MENU;
        return;
    }

    while (true)
    {
        int x = Random::GetNumber(0, levelResolution.x);
        int y = Random::GetNumber(0, levelResolution.y);
        foodController->gameObject->transform.position = glm::vec2(x * unitSize.x, y * unitSize.y);

        if (!playerController->IsInSnake(foodController->gameObject->transform.position))
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