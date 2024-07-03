#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Learning2DEngine/System/Game.h"
#include "Learning2DEngine/System/Text.h"
#include "Learning2DEngine/System/GameObject.h"

#include "GameLevel.h"
#include "PowerUpController.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// <collision?, what direction?, difference vector center - closest point>
typedef std::tuple<bool, Direction, glm::vec2> Collision; 

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

class Breakout : public virtual Learning2DEngine::System::Game
{
private:
    GameState State;
    std::vector<PowerUpController*> PowerUps;
    std::vector<GameLevel> Levels;
    unsigned int Level;
    unsigned int Lives;

    Learning2DEngine::System::Text  liveText;
    Learning2DEngine::System::Text  startText;
    Learning2DEngine::System::Text  levelSelectorText;
    Learning2DEngine::System::Text  winText;
    Learning2DEngine::System::Text  retryText;
protected:
    void Update() override;
    void Render() override;
    void ProcessInput();
    void DoCollisions();

    void ResetLevel();
    void ResetPlayer();

    void SpawnPowerUps(Learning2DEngine::System::GameObject& block);
    void UpdatePowerUps();
    void ClearPowerUps();
public:
    Breakout();
    ~Breakout();

    void Init() override;
};