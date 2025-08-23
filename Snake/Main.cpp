#include "Snake.h"
#include <Learning2DEngine/Render/Resolution.h>
#include <Learning2DEngine/Render/WindowType.h>

using namespace Learning2DEngine::Render;

const Resolution resolution(600, 600);
Snake snakeGame;

int main()
{
    snakeGame.InitWithRender(3, 3, resolution, "Snake", WindowType::FIXED_SIZE);
    snakeGame.Run();
    snakeGame.Terminate();
    return 0;
}