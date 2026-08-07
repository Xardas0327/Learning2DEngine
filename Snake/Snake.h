#pragma once

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/UI/FontSizePair.h>

#include "GameController.h"

class Snake : public Learning2DEngine::System::Game
{
private:
    const Learning2DEngine::UI::FontSizePair fontSizePair;

    GameController* gameController;
protected:
    void Init() override;

public:
    Snake();
    ~Snake();

    void Terminate() override;
};