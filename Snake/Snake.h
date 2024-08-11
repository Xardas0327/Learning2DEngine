#pragma once

#include <list>

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/UI/Text.h>

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

class Snake : public virtual Learning2DEngine::System::Game
{
private:
    const glm::ivec2 levelResolution;
    const Learning2DEngine::UI::FontSizePair fontSizePair;
    //In Seconds
    const float startMoveWaitingTime;
    const float dencreaseTimeAfterFood;
    const unsigned int baseSnakeLength;

    GameState state;
    unsigned int score;
    glm::ivec2 unitSize;
    std::list<Learning2DEngine::System::GameObject*> player;
    Learning2DEngine::System::GameObject* food;
    //In Seconds
    float moveWaitingTime;
    float actualWaitingTime;
    Direction moveDirection;
    Direction lastMoveDirection;

    Learning2DEngine::UI::Text  scoreText;
    Learning2DEngine::UI::Text  startText;
protected:
    void InitSystem();
    void InitObjects();
    void Update() override;
    void Render() override;
    void LateRender() override;

    void ProcessInput();
    void ResetLevel();
    void MoveSnake();
    bool IsOut(const glm::vec2 position);
    bool IsInSnake(const glm::vec2 position);
    void GenerateNextFood();
    void EatFood();
    void RefreshScore();
    Learning2DEngine::System::GameObject* CreateNewPlayerUnit(const glm::vec2 position);
public:
    Snake();
    ~Snake();

    void Init() override;
    void Terminate() override;
};