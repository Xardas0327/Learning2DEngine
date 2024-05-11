#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "Learning2DEngine/System/ResourceManager.h"
#include "Learning2DEngine/Render/RenderManager.h"

using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::System;

// GLFW function declarations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void update_callback(float deltaTime);
void render_callback();

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main()
{
    auto& renderManager = RenderManager::GetInstance();

    renderManager.Init(3, 3, SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout");
    renderManager.AddKeyboardEvent(key_callback);
    renderManager.AddUpdateEvent(update_callback);
    renderManager.AddRenderEvent(render_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    // ---------------
    Breakout.Init();

    renderManager.Run();

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::GetInstance().Clear();

    renderManager.Terminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        RenderManager::GetInstance().CloseWindow();
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.Keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            Breakout.Keys[key] = false;
            Breakout.KeysProcessed[key] = false;
        }
    }
}

void update_callback(float deltaTime)
{
    // manage user input
    // -----------------
    Breakout.ProcessInput(deltaTime);

    // update game state
    // -----------------
    Breakout.Update(deltaTime);
}

void render_callback()
{
    Breakout.Render();
}