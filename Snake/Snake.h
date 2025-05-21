#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Game.h>
#include <Learning2DEngine/UI/FontSizePair.h>

#include "GameController.h"

class Snake : public Learning2DEngine::System::Game
{
private:
    const Learning2DEngine::UI::FontSizePair fontSizePair;

    GameController* gameController;
public:
    Snake();
    ~Snake();

    void Init() override;
    void Terminate() override;
};