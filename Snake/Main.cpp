#include "Snake.h"

#include <Learning2DEngine/System/GameConfig.h>

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;

const Resolution resolution(600, 600);
Snake snakeGame;

int main()
{
	snakeGame.InitByConfig(
        GameConfig("Snake", resolution, 3, 3, WindowType::FIXED_SIZE, false)
    );
    snakeGame.Run();
    snakeGame.Terminate();
    return 0;
}