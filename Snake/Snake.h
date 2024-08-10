#pragma once

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/UI/Text.h>

enum GameState {
    GAME_MENU,
    GAME_ACTIVE
};

class Snake : public virtual Learning2DEngine::System::Game
{
private:
    GameState state;
    unsigned int score;
    const Learning2DEngine::UI::FontSizePair fontSizePair;

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
public:
    Snake();
    ~Snake();

    void Init() override;
    void Terminate() override;
};