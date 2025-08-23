#include "TheCollector.h"
#include <Learning2DEngine/Render/Resolution.h>
#include <Learning2DEngine/Render/WindowType.h>

using namespace Learning2DEngine::Render;

const Resolution resolution(1280, 720);
TheCollector game;

int main()
{
    game.InitWithRender(3, 3, resolution, "The Collector", WindowType::RESIZABLE);
    game.Run();
    game.Terminate();
    return 0;
}