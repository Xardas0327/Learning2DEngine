#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Camera.h"
#include "GameConfig.h"
#include "../EventSystem/ResolutionEventItem.h"
#include "../Render/IResolutionRefresher.h"
#include "../Render/Resolution.h"
#include "../Render/Shader.h"
#include "../Render/MSAA.h"
#include "../Render/PostProcessEffect.h"

namespace Learning2DEngine
{
    namespace System
    {
        /*
            The Function order in the Run() (in a frame):
            Calculate deltaTime
            Refresh Events (Keyboard, Mouse, Window etc)
            Update
            Check Collisions
            LateUpdate
            Clear Window to default color
            Render (with MSAA and PostProcessEffect, if they are enabled)
            LateRender (without any effect)
            Update Window
            Destroy Marked GameObjects
        */
        class Game : protected Render::IResolutionRefresher
        {
        private:
            bool isMsaaActive;
            bool isPostProcessEffectActive;
            bool isPostProcessEffectUsed;
            Render::MSAA msaaRender;
            Render::PostProcessEffect ppeRender;
            EventSystem::ResolutionEventItem resolutionEventItem;
            
        protected:
            Game();

            virtual void Init() = 0;

            void ActivateMSAA(unsigned int sampleNumber);
            void StopMSAA();
            inline bool IsMsaaActive()
            {
                return isMsaaActive;
            }

            void ActivatePostProcessEffect();
            void StopPostProcessEffect();
            inline bool IsPostProcessEffectActive()
            {
                return isPostProcessEffectActive;
            }

            inline void UsePostProcessEffect()
            {
                isPostProcessEffectUsed = true;
            }

            void UsePostProcessEffect(const Render::Shader& shader);

            inline void NotUsePostProcessEffect()
            {
                isPostProcessEffectUsed = false;
            }
            void ClearPostProcessEffect();


            inline bool IsPostProcessEffectUsed()
            {
                return isPostProcessEffectUsed;
            }

            /// <summary>
            /// If this function is override, it must call the Game::RefreshResolution(const Resolution& resolution) in the first line.
            /// </summary>
            virtual void RefreshResolution(const Render::Resolution& resolution) override;
        public:
            virtual ~Game();

            /// <summary>
            /// Its resolution has to be initialized 
            /// It is (0,0) by default.
            /// </summary>
            static Camera mainCamera;

            /// <summary>
            /// It will initialize the RenderManager and the AudioManager (if the config specifies).
            /// The Blend is enable by default with Source: SRC_ALPHA, Destination: ONE_MINUS_SRC_ALPHA,
            /// because it is used for some features.
            /// After that, the Init() will be called.
            /// </summary>
            void InitByConfig(const GameConfig& config);

            /// <summary>
            /// If this function is override, it must call the Game::Terminate() in the last line.
            /// </summary>
            virtual void Terminate();
            void Run();
        };
    }
}