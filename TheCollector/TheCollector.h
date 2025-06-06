#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Game.h>

class TheCollector : public Learning2DEngine::System::Game
{
public:
    TheCollector() = default;
    ~TheCollector() = default;

    void Init() override;
};