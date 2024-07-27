#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <irrklang/irrKlang.h>

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/UI/Text.h>

#include "Script/GameLevel.h"
#include "Script/BackgroundController.h"
#include "Script/PlayerController.h"
#include "Script/BallController.h"
#include "Script/PostProcessData.h"
#include "Script/PowerUpController.h"

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

class Breakout : public virtual Learning2DEngine::System::Game
{
private:
    GameState state;
    std::vector<PowerUpController*> powerUps;
    std::vector<GameLevel> levels;
    unsigned int selectedLevel;
    unsigned int lives;
    BackgroundController* backgroundController;
    PlayerController* playerController;
    BallController* ballController;
    irrklang::ISoundEngine* soundEngine;
    const Learning2DEngine::UI::FontSizePair fontSizePair;
    PostProcessData* postProcessData;
    float shakeTime;

    Learning2DEngine::UI::Text  liveText;
    Learning2DEngine::UI::Text  startText;
    Learning2DEngine::UI::Text  levelSelectorText;
    Learning2DEngine::UI::Text  winText;
    Learning2DEngine::UI::Text  retryText;
protected:
    void InitSystem();
    void InitObjects();
    void Update() override;
    void Render() override;
    void LateRender() override;

    void ProcessInput();
    void DoCollisions();
    void ShakeScreen();
    void IsLiveLost();
    void IsLevelCompleted();

    void ResetLevel();
    void ResetPlayer();

    void SpawnPowerUps(Learning2DEngine::System::GameObject& block);
    void ActivatePowerUp(PowerUpController& powerUp);
    void UpdatePowerUps();
    void ClearPowerUps();
public:
    Breakout();
    ~Breakout();

    void Init() override;
    void Terminate() override;
};