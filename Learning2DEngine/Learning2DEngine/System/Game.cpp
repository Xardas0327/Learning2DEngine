#include "Game.h"

#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "../Render/RenderManager.h"
#include "../DebugTool/DebugMacro.h"
#include "../DebugTool/Log.h"
#include "ResourceManager.h"
#include "ComponentManager.h"

namespace Learning2DEngine
{
    using namespace Render;

    namespace System
    {
        float Game::deltaTime = 0.0f;
        Camera Game::mainCamera = Camera();
        InputStatus Game::keyboardButtons[L2DE_KEYBOARD_BUTTON_NUMBER] = { InputStatus::KEY_UP };

        Game::Game()
            : lastFrame(0.0f), timeScale(L2DE_TIME_SCALE_DEFAULT), isMsaaActive(false),
            isPostProcessEffectActive(false), isPostProcessEffectUsed(false), msaaRender(),
            ppeRender(), keyboardEventItem(this), resolutionEventItem(this)
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
            renderManager.AddKeyboardEvent(&keyboardEventItem);
            renderManager.AddFramebufferSizeEvent(&resolutionEventItem);
            renderManager.EnableBlend();
            renderManager.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        void Game::Terminate()
        {
            StopMSAA();
            StopPostProcessEffect();

            auto& renderManager = RenderManager::GetInstance();
            renderManager.RemoveKeyboardEvent(&keyboardEventItem);
            renderManager.RemoveFramebufferSizeEvent(&resolutionEventItem);

            ComponentManager::GetInstance().Clear();
            ResourceManager::GetInstance().Clear();
            RenderManager::GetInstance().Terminate();
        }

        void Game::Run()
        {
            try
            {
                auto& renderManager = RenderManager::GetInstance();
				auto& componentManager = ComponentManager::GetInstance();
                while (!renderManager.IsWindowClosed())
                {
                    // Calc deltaTime
                    float currentFrame = glfwGetTime();
                    Game::deltaTime = (currentFrame - lastFrame);
                    lastFrame = currentFrame;
#if L2DE_DEBUG
                    // It is useful if the game stop by a breakpoint
                    if (Game::deltaTime > L2DE_DEBUG_MAX_BASE_DELTATIME)
                        Game::deltaTime = L2DE_DEBUG_MAX_BASE_DELTATIME;
#endif
                    Game::deltaTime *= timeScale;

                    UpdateEvents();

                    componentManager.Update();
                    componentManager.CheckCollision();
                    componentManager.LateUpdate();

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

                    componentManager.Render();

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

				    componentManager.LateRender();

                    renderManager.UpdateWindow();
                }
            }
            catch (std::exception e)
            {
                L2DE_LOG_ERROR(std::string("GAME: Unhandled Exception: ") +  e.what());
            }
            catch (...)
            {
                L2DE_LOG_ERROR(std::string("GAME: Unknown Exception."));
            }
        }

        void Game::ActivateMSAA(unsigned int sampleNumber)
        {
            if (isMsaaActive)
            {
                L2DE_LOG_WARNING("Game: The MSAA was activated with " + std::to_string(msaaRender.GetSampleNumber())
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

        void Game::UpdateEvents()
        {
            FixKeyboardButtons();
            glfwPollEvents();
        }

        void Game::RefreshKeyboard(int key, int scancode, int action, int mode)
        {
            if (key > GLFW_KEY_UNKNOWN && key < L2DE_KEYBOARD_BUTTON_NUMBER)
            {
                switch (action)
                {
                case GLFW_RELEASE:
                    keyboardButtons[key] = InputStatus::KEY_UP;
                    break;
                case GLFW_PRESS:
                    keyboardButtons[key] = InputStatus::KEY_DOWN;
                    break;
                case GLFW_REPEAT:
                    keyboardButtons[key] = InputStatus::KEY_HOLD;
                    break;
                default:
                    L2DE_LOG_ERROR("GAME: Unknow input action: " + action);
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

        void Game::FixKeyboardButtons()
        {
            for (int i = 0; i < L2DE_KEYBOARD_BUTTON_NUMBER; ++i)
            {
                if(Game::keyboardButtons[i] == InputStatus::KEY_DOWN)
                    Game::keyboardButtons[i] = InputStatus::KEY_HOLD;
            }
        }
    }
}