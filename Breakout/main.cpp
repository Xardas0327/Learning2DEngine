#include "Breakout.h"

#include <Learning2DEngine/System/GameConfig.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;

const Resolution resolution(800, 600);
Breakout breakoutGame;

int main()
{
    breakoutGame.InitByConfig(GameConfig("Breakout", resolution));
    breakoutGame.Run();
    breakoutGame.Terminate();
    return 0;
}