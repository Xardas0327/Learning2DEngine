#include <Learning2DEngine/Render/Renderer.h>
#include "Breakout.h"

using namespace Learning2DEngine::Render;

const Resolution resolution(800, 600);
Breakout breakoutGame;

int main()
{
    breakoutGame.InitWithRender(3, 3, resolution, "Breakout");
    breakoutGame.Run();
    breakoutGame.Terminate();
    return 0;
}