#pragma once

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
    GameState state;
    unsigned int score;
    const glm::ivec2 levelResolution;
    glm::ivec2 unitSize;
    const Learning2DEngine::UI::FontSizePair fontSizePair;
    Learning2DEngine::System::GameObject* player;
    Learning2DEngine::System::GameObject* food;
    //In Seconds
    float waitingTime;
    //In Seconds
    const float startWaitingTime;
    Direction moveDirection;

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
    void GenerateNextFood();
    void IsFoodEated();
    void RefreshScore();
public:
    Snake();
    ~Snake();

    void Init() override;
    void Terminate() override;
};