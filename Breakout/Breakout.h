#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/UI/Text.h>

#include "GameLevel.h"
#include "PowerUpController.h"
#include "Script/BackgroundController.h"

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
typedef std::tuple<bool, Direction, glm::vec2> CollisionResult;

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);
const float BALL_RADIUS = 12.5f;

class Breakout : public virtual Learning2DEngine::System::Game
{
private:
    GameState state;
    std::vector<PowerUpController*> powerUps;
    std::vector<GameLevel> levels;
    unsigned int selectedLevel;
    unsigned int lives;
    BackgroundController* backgroundController;

    Learning2DEngine::UI::Text  liveText;
    Learning2DEngine::UI::Text  startText;
    Learning2DEngine::UI::Text  levelSelectorText;
    Learning2DEngine::UI::Text  winText;
    Learning2DEngine::UI::Text  retryText;
protected:
    void Update() override;
    void Render() override;
    void LateRender() override;
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
    void Terminate() override;
};