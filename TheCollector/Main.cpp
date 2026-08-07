#include "TheCollector.h"

#include <Learning2DEngine/System/GameConfig.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;

const Resolution resolution(1280, 720);
TheCollector game;

int main()
{
    game.InitByConfig(GameConfig("The Collector", resolution));
    game.Run();
    game.Terminate();
    return 0;
}