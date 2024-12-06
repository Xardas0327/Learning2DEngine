#include "Game.h"

#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "../Render/RenderManager.h"
#include "../DebugTool/Log.h"
#include "ResourceManager.h"

namespace Learning2DEngine
{
    using namespace Render;

    namespace System
    {
        float Game::deltaTime = 0.0f;
        Camera Game::mainCamera = Camera();

        Game::Game()
            : lastFrame(0.0f), timeScale(TIME_SCALE_DEFAULT), isMsaaActive(false),
            isPostProcessEffectActive(false), isPostProcessEffectUsed(false), msaaRender(),
            ppeRender(), keyboardMouseEventItem(this), resolutionEventItem(this), inputKeys()
        {
        }

        Game::~Game()
        {

        }

        void Game::InitWithRender(int majorRenderVersion, int minorRenderVersion, Resolution resolution, const char* title, bool resizableWindows)
        {
            RenderManager::GetInstance().Init(majorRenderVersion, minorRenderVersion, resolution, title, resizableWindows);
            Init();
        }

        void Game::Init()
        {
            auto& renderManager = RenderManager::GetInstance();
            renderManager.AddKeyboardEvent(&keyboardMouseEventItem);
            renderManager.AddFramebufferSizeEvent(&resolutionEventItem);
            renderManager.EnableBlend();
            renderManager.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            for (int i = 0; i < INPUT_KEY_SIZE; ++i)
            {
                inputKeys[i] = InputStatus::KEY_UP;
            }
        }

        void Game::Terminate()
        {
            StopMSAA();
            StopPostProcessEffect();

            auto& renderManager = RenderManager::GetInstance();
            renderManager.RemoveKeyboardEvent(&keyboardMouseEventItem);
            renderManager.RemoveFramebufferSizeEvent(&resolutionEventItem);

            ResourceManager::GetInstance().Clear();
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
                    bool usePPE = isPostProcessEffectActive && isPostProcessEffectUsed;
                    if (isMsaaActive)
                    {
                        msaaRender.StartRender();
                    }
                    else if (usePPE)
                    {
                        ppeRender.StartRender();
                    }
                    Render();
                    if (isMsaaActive)
                    {
                        msaaRender.EndRender(
                            usePPE ? ppeRender.GetFrameBufferId() : 0,
                            renderManager.GetResolution());
                    }
                    else if (usePPE)
                    {
                        ppeRender.EndRender();
                    }

                    if (usePPE)
                        ppeRender.Render();

                    LateRender();

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

        void Game::ActivateMSAA(unsigned int sampleNumber)
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

        void Game::ActivatePostProcessEffect()
        {
            if (isPostProcessEffectActive)
                return;

            isPostProcessEffectActive = true;
            ppeRender.Init(RenderManager::GetInstance().GetResolution());
        }

        void Game::StopPostProcessEffect()
        {
            if (!isPostProcessEffectActive)
                return;

            isPostProcessEffectActive = false;
            ppeRender.Destroy();
        }

        void Game::UsePostProcessEffect(const Render::Shader& shader)
        {
            UsePostProcessEffect();
            ppeRender.SetShader(shader);
        }

        void Game::ClearPostProcessEffect()
        {
            NotUsePostProcessEffect();
            ppeRender.ClearShader();
        }

        void Game::UpdateKeyboardMouseEvents()
        {
            FixKeyboardMouse();
            glfwPollEvents();
        }

        void Game::RefreshKeyboardMouse(int key, int scancode, int action, int mode)
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

        void Game::RefreshResolution(const Resolution& resolution)
        {
            if(isMsaaActive)
            {
                unsigned int sampleNumber = msaaRender.GetSampleNumber();
                msaaRender.Destroy();
                msaaRender.Init(sampleNumber, resolution);
            }

            if (isPostProcessEffectActive)
            {
                ppeRender.Destroy();
                ppeRender.Init(resolution);
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