#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/UI/FontSizePair.h>

#include "Script/GameController.h"
#include "Script/PostProcessData.h"

class Breakout : public Learning2DEngine::System::Game
{
private:
    const Learning2DEngine::UI::FontSizePair fontSizePair;
    PostProcessData* postProcessData;
    GameController* gameController;

protected:
    void Init() override;

public:
    Breakout();
    ~Breakout();

    void Terminate() override;
};