#include "Breakout.h"

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

Breakout breakoutGame;

int main()
{
    breakoutGame.InitWithRender(3, 3, SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout");
    breakoutGame.Run();
    breakoutGame.Terminate();
    return 0;
}