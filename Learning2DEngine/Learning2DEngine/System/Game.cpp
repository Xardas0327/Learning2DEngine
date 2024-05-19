#include "Game.h"

#include "../Render/RenderManager.h"

using namespace Learning2DEngine::Render;

namespace Learning2DEngine
{
    namespace System
    {
        Game::Game() :
            deltaTime(0.0f), lastFrame(0.0f)
        {
        }

        Game::~Game()
        {

        }

        void Game::Init(int majorRenderVersion, int minorRenderVersion, int screenWidth, int screenHeight, const char* title)
        {
            RenderManager::GetInstance().Init(majorRenderVersion, minorRenderVersion, screenWidth, screenHeight, title);
            Init();
        }

        void Game::Init()
        {
            auto& renderManager = RenderManager::GetInstance();
            renderManager.AddKeyboardEvent(CallbackRefreshKeyboard);
        }

        void Game::Terminate()
        {
            RenderManager::GetInstance().Terminate();
        }

        void Game::Run()
        {
            auto& renderManager = RenderManager::GetInstance();
            while (!renderManager.IsWindowClosed())
            {
                // Calc deltaTime
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                //Refresh Keyboard and Mouse events
                glfwPollEvents();

                ProcessInput(deltaTime);
                Update(deltaTime);

                renderManager.ClearScreen();
                Render();

                renderManager.UpdateScreen();
            }
        }

        void Game::CallbackRefreshKeyboard(int key, int scancode, int action, int mode)
        {
            if (key >= 0 && key < 1024)
            {
                switch (action)
                {
                case GLFW_RELEASE:
                    inputKeys[key] = InputStatus::KEY_UP;
                    break;
                case GLFW_PRESS:
                    inputKeys[key] = InputStatus::KEY_DOWN;
                    break;
                case GLFW_REPEAT:
                    inputKeys[key] = InputStatus::KEY_STAY;
                    break;
                //default:
                //    std::string message = "ERROR::GAME: Unknow action: " + action;
                //    throw new exception(message);
                }
            }
        }
    }
}