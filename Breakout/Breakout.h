#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/UI/FontSizePair.h>

#include "Script/GameController.h"
#include "Script/PostProcessData.h"

class Breakout : public virtual Learning2DEngine::System::Game
{
private:
    const Learning2DEngine::UI::FontSizePair fontSizePair;
    PostProcessData* postProcessData;
    GameController* gameController;

public:
    Breakout();
    ~Breakout();

    void Init() override;
    void Terminate() override;
};