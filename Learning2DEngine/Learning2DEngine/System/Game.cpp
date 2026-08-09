#include "Game.h"

#include <string>

#include "../Render/RenderManager.h"
#include "../DebugTool/Log.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "ComponentManager.h"
#include "GameObjectManager.h"
#include "Time.h"

namespace Learning2DEngine
{
    using namespace Render;

    namespace System
    {
        Camera Game::mainCamera = Camera();

        Game::Game()
            : isMsaaActive(false), isPostProcessEffectActive(false), isPostProcessEffectUsed(false), msaaRender(),
            ppeRender(), resolutionEventItem(this)
        {
        }

        Game::~Game()
        {

        }

        void Game::InitByConfig(const GameConfig& config)
        {
            auto& renderManager = RenderManager::GetInstance();
            renderManager.Init(config.openGLMajorVersion, config.openGLMinorVersion, config.resolution, config.title, config.windowType);
            renderManager.AddFramebufferSizeEvent(&resolutionEventItem);
            //Because of images' alpha channel
            renderManager.EnableBlend();
            renderManager.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			InputManager::GetInstance().Init();

            if(config.initAudioManager)
			    AudioManager::GetInstance().Init(config.audioManagerConfig);

			Init();

        }

        void Game::Terminate()
        {
            StopMSAA();
            StopPostProcessEffect();

            auto& renderManager = RenderManager::GetInstance();
            renderManager.RemoveFramebufferSizeEvent(&resolutionEventItem);

			InputManager::GetInstance().Terminate();

            GameObjectManager::GetInstance().DestroyAllGameObjects();
            ComponentManager::GetInstance().Clear();
            ResourceManager::GetInstance().Clear();
            RenderManager::GetInstance().Terminate();
            AudioManager::GetInstance().Terminate();
        }

        void Game::Run()
        {
            try
            {
                auto& renderManager = RenderManager::GetInstance();
                auto& gameObjectManager = GameObjectManager::GetInstance();
                auto& componentManager = ComponentManager::GetInstance();
                auto& inputManager = InputManager::GetInstance();


                while (!renderManager.IsWindowClosed())
                {
                    Time::CalcDeltaTime();

                    inputManager.UpdateEvents();

                    componentManager.Update();
                    componentManager.CheckCollision();
                    componentManager.LateUpdate();

                    renderManager.ClearWindow();

                    componentManager.SetDataToRenderers();

                    bool usePPE = isPostProcessEffectActive && isPostProcessEffectUsed;

                    // Change the framebuffer to the MSAA and/or PostProcessEffect
                    if (isMsaaActive)
                    {
                        msaaRender.StartRender();
                    }
                    else if (usePPE)
                    {
                        ppeRender.StartRender();
                    }

                    componentManager.Render();

                    // Change the framebuffer back to the default
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

                    gameObjectManager.DestroyMarkedGameObjects();
                }
            }
            catch (std::exception e)
            {
                L2DE_LOG_ERROR(std::string("GAME: Unhandled Exception: ") + e.what())
            }
            catch (...)
            {
                L2DE_LOG_ERROR(std::string("GAME: Unknown Exception."))
            }
        }

        void Game::ActivateMSAA(unsigned int sampleNumber)
        {
            if (isMsaaActive)
            {
                L2DE_LOG_WARNING("Game: The MSAA was activated with " + std::to_string(msaaRender.GetSampleNumber())
                    + " samples. That is why the Game does not activated the MSAA with " + std::to_string(sampleNumber))
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

        void Game::RefreshResolution(const Resolution& resolution)
        {
            if (resolution.GetWidth() <= 0 || resolution.GetHeight() <= 0)
                return;

            if (isMsaaActive)
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
    }
}