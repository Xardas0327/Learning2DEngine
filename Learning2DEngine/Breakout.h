#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Learning2DEngine/System/Game.h"
#include "Learning2DEngine/UI/Text.h"

#include "game_level.h"
#include "power_up.h"

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
    GameState                   State;
    std::vector<GameLevel>      Levels;
    unsigned int                Level;
    std::vector<PowerUp>        PowerUps;
    unsigned int                Lives;
    Learning2DEngine::UI::Text  liveText;
    Learning2DEngine::UI::Text  startText;
    Learning2DEngine::UI::Text  levelSelectorText;
    Learning2DEngine::UI::Text  winText;
    Learning2DEngine::UI::Text  retryText;
protected:
    void Update(float deltaTime) override;
    void Render() override;
    void ProcessInput(float deltaTime);
    void DoCollisions();

    void ResetLevel();
    void ResetPlayer();

    void SpawnPowerUps(GameObject& block);
    void UpdatePowerUps(float deltaTime);
public:
    Breakout();
    ~Breakout();

    void Init() override;
    void Terminate() override;
};