#include <Learning2DEngine/Render/Renderer.h>
#include "Snake.h"

using namespace Learning2DEngine::Render;

const Resolution resolution(600, 600);
Snake snakeGame;

int main()
{
    snakeGame.InitWithRender(3, 3, resolution, "Snake", false);
    snakeGame.Run();
    snakeGame.Terminate();
    return 0;
}