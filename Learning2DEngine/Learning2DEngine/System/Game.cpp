#include "Game.h"

#include "../Render/RenderManager.h"
#include "Log.h"

namespace Learning2DEngine
{
    using namespace Render;

    namespace System
    {
        InputStatus Game::inputKeys[INPUT_KEY_SIZE] = { InputStatus::KEY_UP };

        Game::Game() :
            deltaTime(0.0f), lastFrame(0.0f), timeScale(TIME_SCALE_DEFAULT)
        {
        }

        Game::~Game()
        {

        }

        void Game::InitWithRender(int majorRenderVersion, int minorRenderVersion, Resolution resolution, const char* title)
        {
            RenderManager::GetInstance().Init(majorRenderVersion, minorRenderVersion, resolution, title);
            Init();
        }

        void Game::Init()
        {
            RenderManager::GetInstance().AddKeyboardEvent(CallbackRefreshKeyboardMouse);
        }

        void Game::Terminate()
        {
            RenderManager::GetInstance().Terminate();
        }

        void Game::Run()
        {
            try
            {
                auto& renderManager = RenderManager::GetInstance();
                while (!renderManager.IsWindowClosed())
                {
                    // Calc deltaTime
                    float currentFrame = glfwGetTime();
                    deltaTime = currentFrame - lastFrame;
                    lastFrame = currentFrame;

                    UpdateKeyboardMouseEvents();

                    Update(deltaTime * timeScale);

                    renderManager.ClearScreen();
                    Render();

                    renderManager.UpdateScreen();
                }
            }
            catch (std::exception e)
            {
                LOG_ERROR(std::string("GAME: Unhandled Exception: ") +  e.what());
            }
            catch (...)
            {
                LOG_ERROR(std::string("GAME: Unknown Exception."));
            }
        }

        void Game::UpdateKeyboardMouseEvents()
        {
            FixKeyboardMouse();
            glfwPollEvents();
        }

        void Game::CallbackRefreshKeyboardMouse(int key, int scancode, int action, int mode)
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
                    inputKeys[key] = InputStatus::KEY_HOLD;
                    break;
                default:
                    LOG_WARNING("GAME: Unknow input action: " + action);
                }
            }
        }

        void Game::FixKeyboardMouse()
        {
            for (int i = 0; i < INPUT_KEY_SIZE; ++i)
            {
                if(inputKeys[i] == InputStatus::KEY_DOWN)
                    inputKeys[i] = InputStatus::KEY_HOLD;
            }
        }
    }
}