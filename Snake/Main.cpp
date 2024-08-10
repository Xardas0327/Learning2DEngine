#include <Learning2DEngine/Render/Renderer.h>
#include "Snake.h"

using namespace Learning2DEngine::Render;

const Resolution resolution(800, 800);
Snake snakeGame;

int main()
{
    snakeGame.InitWithRender(3, 3, resolution, "Snake");
    snakeGame.Run();
    snakeGame.Terminate();
    return 0;
}