#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Game.h>

class TheCollector : public Learning2DEngine::System::Game
{
protected:
    void Init() override;

public:
    TheCollector() = default;
    ~TheCollector() = default;
};