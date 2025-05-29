#include "GameController.h"

#include <Learning2DEngine/DebugTool/DebugMacro.h>
#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/InputStatus.h>
#include <Learning2DEngine/System/Random.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/Render/RenderManager.h>
#include <Learning2DEngine/Object/FpsShower.h>

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::UI;
using namespace Learning2DEngine::Object;

GameController::GameController(GameObject* gameObject, const FontSizePair& fontSizePair)
	: UpdaterComponent(gameObject), Component(gameObject),
	levelResolution(MAP_SIZE), fontSizePair(fontSizePair), startMoveWaitingTime(START_MOVE_WAITING_TIME),
	dencreaseTimeAfterEat(TIME_DECREASE), baseSnakeLength(3),
	state(GameState::GAME_MENU), score(0), unitSize(0), foodController(nullptr), playerController(nullptr),
	moveWaitingTime(0.0f), actualWaitingTime(0.0f), moveDirection(), lastMoveDirection(), scoreText(nullptr), startText(nullptr)
{

}

void GameController::Init()
{
    UpdaterComponent::Init();

    const Resolution resolution = RenderManager::GetInstance().GetResolution();

    // Unit
    unitSize = glm::ivec2(resolution.GetWidth() / levelResolution.x, resolution.GetHeight() / levelResolution.y);

    //Player
    auto player = GameObject::Create();
    playerController = player->AddComponent<PlayerController>("Unit");

    //Food
    auto food = GameObject::Create(
        Transform(
            glm::vec2(0.0f, 0.0f),
            unitSize
        )
    );
    foodController = food->AddComponent<FoodController>("Unit");


    // Text
    auto scoreGameObject = GameObject::Create(
        Transform(
            glm::vec2(5.0f, 5.0f)
        )
    );
    scoreText = scoreGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "Score: " + std::to_string(score)
    );

    auto startGameObject = GameObject::Create(
        Transform(
            glm::vec2(175.0f, static_cast<float>(resolution.GetHeight() / 2))
        )
    );
    startText = startGameObject->AddComponent<SimpleText2DRenderComponent>(
        RendererMode::LATERENDER,
        fontSizePair,
        "Press ENTER to start"
    );

#if L2DE_DEBUG
    FpsShower::CreateFpsShowerObject(
        Transform(
            glm::vec2(5.0f, resolution.GetHeight() - 30)
        ),
        fontSizePair,
        99);
#endif

    ResetLevel();
}

void GameController::Update()
{
    ProcessInput();

    MoveSnake();
}

void GameController::ProcessInput()
{
    if (Game::GetKeyboardButtonStatus(GLFW_KEY_ESCAPE) == InputStatus::KEY_DOWN)
    {
        RenderManager::GetInstance().CloseWindow();
        return;
    }

    switch (state)
    {
    case GameState::GAME_MENU:
        if (Game::GetKeyboardButtonStatus(GLFW_KEY_ENTER) == InputStatus::KEY_DOWN)
        {
            state = GameState::GAME_ACTIVE;
            startText->gameObject->isActive = false;
            ResetLevel();
        }
    case GameState::GAME_ACTIVE:
        if (lastMoveDirection == Direction::LEFT || lastMoveDirection == Direction::RIGHT)
        {
            if (Game::GetKeyboardButtonStatus(GLFW_KEY_W) == InputStatus::KEY_DOWN
                || Game::GetKeyboardButtonStatus(GLFW_KEY_UP) == InputStatus::KEY_DOWN)
            {
                moveDirection = Direction::UP;
            }
            else if (Game::GetKeyboardButtonStatus(GLFW_KEY_S) == InputStatus::KEY_DOWN
                || Game::GetKeyboardButtonStatus(GLFW_KEY_DOWN) == InputStatus::KEY_DOWN)
            {
                moveDirection = Direction::DOWN;
            }
        }
        else if (lastMoveDirection == Direction::UP || lastMoveDirection == Direction::DOWN)
        {
            if (Game::GetKeyboardButtonStatus(GLFW_KEY_A) == InputStatus::KEY_DOWN
                || Game::GetKeyboardButtonStatus(GLFW_KEY_LEFT) == InputStatus::KEY_DOWN)
            {
                moveDirection = Direction::LEFT;
            }
            else if (Game::GetKeyboardButtonStatus(GLFW_KEY_D) == InputStatus::KEY_DOWN
                || Game::GetKeyboardButtonStatus(GLFW_KEY_RIGHT) == InputStatus::KEY_DOWN)
            {
                moveDirection = Direction::RIGHT;
            }
        }
        break;
    }
}

void GameController::ResetLevel()
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

void GameController::MoveSnake()
{
    if (state != GameState::GAME_ACTIVE)
        return;

    actualWaitingTime -= Time::GetDeltaTime();
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
        glm::vec2 newPosition = playerController->GetHeadPosition() + glm::vec2(moveVector.x * unitSize.x, moveVector.y * unitSize.y);

        if (IsOut(newPosition) || playerController->IsInSnake(newPosition))
        {
            state = GameState::GAME_MENU;
            startText->gameObject->isActive = true;
        }
        else
        {
            if (newPosition == foodController->gameObject->transform.GetPosition())
            {
                playerController->IncreaseSize(newPosition, unitSize);
                EatFood();
                moveWaitingTime -= dencreaseTimeAfterEat;
                if (moveWaitingTime < 0)
                    moveWaitingTime = 0;
            }
            else
            {
                playerController->Move(newPosition);
            }


            actualWaitingTime += moveWaitingTime;
            if (actualWaitingTime < 0)
                actualWaitingTime = 0;
        }
    }
}

bool GameController::IsOut(const glm::vec2 position)
{
    auto cameraResolution = Game::mainCamera.GetResolution();

    return position.x < 0 || position.x >= cameraResolution.GetWidth()
        || position.y < 0 || position.y >= cameraResolution.GetHeight();
}

void GameController::GenerateNextFood()
{
    if (playerController->GetSize() >= levelResolution.x * levelResolution.y)
    {
        state = GameState::GAME_MENU;
        startText->gameObject->isActive = true;
        return;
    }

    while (true)
    {
        int x = Random::GetNumber(0, levelResolution.x);
        int y = Random::GetNumber(0, levelResolution.y);
		foodController->gameObject->transform.SetPosition(glm::vec2(x * unitSize.x, y * unitSize.y));

        if (!playerController->IsInSnake(foodController->gameObject->transform.GetPosition()))
            break;
    }
}

void GameController::EatFood()
{
    ++score;
    RefreshScore();
    GenerateNextFood();
}

void GameController::RefreshScore()
{
	scoreText->data.text = "Score: " + std::to_string(score);
}