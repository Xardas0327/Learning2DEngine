#include "Game.h"

#include <string>

#include "../Render/RenderManager.h"
#include "../Render/Text2DRenderer.h"
#include "ResourceManager.h"
#include "Log.h"

namespace Learning2DEngine
{
    using namespace Render;

    namespace System
    {
        InputStatus Game::inputKeys[INPUT_KEY_SIZE] = { InputStatus::KEY_UP };
        float Game::deltaTime = 0.0f;
        glm::mat4 Game::cameraProjection = glm::mat4(0.0f);

        Game::Game()
            : lastFrame(0.0f), timeScale(TIME_SCALE_DEFAULT), isMsaaActive(false), msaaRender()
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
            auto& renderManager = RenderManager::GetInstance();
            renderManager.AddKeyboardEvent(Game::CallbackRefreshKeyboardMouse);
            renderManager.EnableBlend();
            renderManager.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            auto& textRenderer = Text2DRenderer::GetInstance();
            textRenderer.Init();
        }

        void Game::Terminate()
        {
            StopMSAA();

            ResourceManager::GetInstance().Clear();
            Text2DRenderer::GetInstance().Terminate();
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
                    Game::deltaTime = (currentFrame - lastFrame) * timeScale;
                    lastFrame = currentFrame;

                    UpdateKeyboardMouseEvents();
                    Update();

                    renderManager.ClearWindow();
                    //MSAA Start will be here
                    Render();
                    //MSAA End will be here
                    renderManager.UpdateWindow();
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

        void Game::UseMSAA(unsigned int sampleNumber)
        {
            if (isMsaaActive)
            {
                LOG_WARNING("Game: The MSAA was activated with " + std::to_string(msaaRender.GetSampleNumber())
                    + " samples. That is why the Game does not activated the MSAA with " + std::to_string(sampleNumber));
                return;
            }

            isMsaaActive = true;
            msaaRender.Init(sampleNumber, RenderManager::GetInstance().GetResolution());
        }

        void Game::StopMSAA()
        {
            if (!isMsaaActive)
                return;

            isMsaaActive = false;
            msaaRender.Destroy();
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
                    Game::inputKeys[key] = InputStatus::KEY_UP;
                    break;
                case GLFW_PRESS:
                    Game::inputKeys[key] = InputStatus::KEY_DOWN;
                    break;
                case GLFW_REPEAT:
                    Game::inputKeys[key] = InputStatus::KEY_HOLD;
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
                if(Game::inputKeys[i] == InputStatus::KEY_DOWN)
                    Game::inputKeys[i] = InputStatus::KEY_HOLD;
            }
        }
    }
}