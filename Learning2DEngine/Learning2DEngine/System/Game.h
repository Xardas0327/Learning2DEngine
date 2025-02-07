#pragma once

#include <glm/glm.hpp>

#include "InputStatus.h"
#include "IKeyboardMouseRefresher.h"
#include "Camera.h"
#include "../EventSystem/KeyboardMouseEventItem.h"
#include "../Render/RenderManager.h"
#include "../Render/IResolutionRefresher.h"
#include "../Render/Resolution.h"
#include "../EventSystem/ResolutionEventItem.h"
#include "../Render/Shader.h"
#include "../Render/MSAA.h"
#include "../Render/PostProcessEffect.h"

namespace Learning2DEngine
{
    namespace System
    {
#define L2DE_INPUT_KEY_SIZE 1024
#define L2DE_TIME_SCALE_DEFAULT 1.0f

        /*
            The Function order in the Run() (in a frame):
            Calculate deltaTime
            Refresh Keyboard and Mouse events
            Update
            Check Collisions
            LateUpdate
            Clear Window to default color
            Render (with MSAA and PostProcessEffect, if they are enabled)
            LateRender (without any effect)
            Update Window
        */
        class Game : public virtual IKeyboardMouseRefresher, public Render::IResolutionRefresher
        {
        private:
            float lastFrame;
            float timeScale;
            bool isMsaaActive;
            bool isPostProcessEffectActive;
            bool isPostProcessEffectUsed;
            Render::MSAA msaaRender;
            Render::PostProcessEffect ppeRender;
            EventSystem::KeyboardMouseEventItem keyboardMouseEventItem;
            EventSystem::ResolutionEventItem resolutionEventItem;

            static InputStatus inputKeys[L2DE_INPUT_KEY_SIZE];

            /// <summary>
            /// It is multiplied by timeScale.
            /// Before the first frame, it is 0.0f.
            /// </summary>
            static float deltaTime;

            void UpdateKeyboardMouseEvents();
            /// <summary>
            /// The glfwPollEvents doesn't refresh the data on every frame.
            /// That's why this function update the InputStatus::KEY_DOWN to InputStatus::KEY_HOLD.
            /// </summary>
            void FixKeyboardMouse();
        protected:
            Game();

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

            inline void ResetTimeScale()
            {
                timeScale = L2DE_TIME_SCALE_DEFAULT;
            }

            inline void SetTimeScale(float value)
            {
                timeScale = value;
            }

            inline float GetTimeScale()
            {
                return timeScale;
            }

        public:
            virtual ~Game();

            /// <summary>
            /// Its resolution has to be initialized 
            /// It is (0,0) by default.
            /// </summary>
            static Camera mainCamera;

            /// <summary>
            /// It will initialize the RenderManager.
            /// After that, the Init() will be called.
            /// </summary>
            void InitWithRender(int majorRenderVersion, int minorRenderVersion, Render::Resolution resolution, const char* title, bool resizableWindows = true);
            /// <summary>
            /// The RenderManager have to be initialized before this.
            /// If this function is override, it must call the Game::Init() in the first line.
            /// The Blend is enable by default with Source: SRC_ALPHA, Destination: ONE_MINUS_SRC_ALPHA,
            /// because it is used for some features.
            /// </summary>
            virtual void Init();
            /// <summary>
            /// If this function is override, it must call the Game::Terminate() in the last line.
            /// </summary>
            virtual void Terminate();
            void Run();

            void RefreshKeyboardMouse(int key, int scancode, int action, int mode) override;
            /// <summary>
            /// If this function is override, it must call the Game::RefreshResolution(const Resolution& resolution) in the first line.
            /// </summary>
            virtual void RefreshResolution(const Render::Resolution& resolution) override;

            static float GetDeltaTime()
            {
                return deltaTime;
            }

            static InputStatus GetInputStatus(int key)
            {
                return inputKeys[key];
            }
        };
    }
}