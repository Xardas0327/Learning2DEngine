#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/Object/FpsShower.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/UpdaterComponent.h>
#include <Learning2DEngine/UI/FontSizePair.h>
#include <Learning2DEngine/UI/Text2DLateRenderer.h>

#include "FoodController.h"
#include "PlayerController.h"

enum GameState {
    GAME_MENU,
    GAME_ACTIVE
};

enum Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

const glm::vec2 VECTOR_UP(0.0f, -1.0f);
const glm::vec2 VECTOR_DOWN(0.0f, 1.0f);
const glm::vec2 VECTOR_RIGHT(1.0f, 0.0f);
const glm::vec2 VECTOR_LEFT(-1.0f, 0.0f);

const glm::ivec2 MAP_SIZE(10, 10);
const float START_MOVE_WAITING_TIME = 0.5f;
const float TIME_DECREASE = 0.005f;

class GameController final : public virtual Learning2DEngine::System::UpdaterComponent
{
    friend class Learning2DEngine::System::GameObject;
private:
    const glm::ivec2 levelResolution;
    const Learning2DEngine::UI::FontSizePair fontSizePair;
    //In Seconds
    const float startMoveWaitingTime;
    const float dencreaseTimeAfterEat;
    const unsigned int baseSnakeLength;

    GameState state;
    unsigned int score;
    glm::ivec2 unitSize;
    FoodController* foodController;
    PlayerController* playerController;
    //In Seconds
    float moveWaitingTime;
    float actualWaitingTime;
    Direction moveDirection;
    Direction lastMoveDirection;

    Learning2DEngine::UI::Text2DLateRenderer* scoreText;
    Learning2DEngine::UI::Text2DLateRenderer* startText;
    Learning2DEngine::Object::FpsShower* fpsShower;


    GameController(Learning2DEngine::System::GameObject* gameObject, const Learning2DEngine::UI::FontSizePair& fontSizePair);

    void ProcessInput();
    void ResetLevel();
    void MoveSnake();
    bool IsOut(const glm::vec2 position);
    void GenerateNextFood();
    void EatFood();
    void RefreshScore();

public:
    void Init() override;
    void Destroy() override;
    void Update() override;
};

