#pragma once

#include <glm/glm.hpp>

#include "InputStatus.h"
#include "Cursor.h"
#include "IKeyboardRefresher.h"
#include "ICursorRefresher.h"
#include "Camera.h"
#include "../EventSystem/KeyboardEventItem.h"
#include "../EventSystem/MouseButtonEventItem.h"
#include "../EventSystem/CursorPositionEventItem.h"
#include "../EventSystem/CursorEnterEventItem.h"
#include "../EventSystem/ScrollEventItem.h"
#include "../EventSystem/ResolutionEventItem.h"
#include "../Render/RenderManager.h"
#include "../Render/IResolutionRefresher.h"
#include "../Render/Resolution.h"
#include "../Render/Shader.h"
#include "../Render/MSAA.h"
#include "../Render/PostProcessEffect.h"

namespace Learning2DEngine
{
    namespace System
    {
#define L2DE_KEYBOARD_BUTTON_NUMBER 512
#define L2DE_TIME_SCALE_DEFAULT 1.0f

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
        */
        class Game : public IKeyboardRefresher,
                    public ICursorRefresher,
                    public Render::IResolutionRefresher
        {
        private:
            float lastFrame;
            float timeScale;
            bool isMsaaActive;
            bool isPostProcessEffectActive;
            bool isPostProcessEffectUsed;
            Render::MSAA msaaRender;
            Render::PostProcessEffect ppeRender;
            EventSystem::KeyboardEventItem keyboardEventItem;
            EventSystem::MouseButtonEventItem mouseButtonEventItem;
            EventSystem::CursorPositionEventItem cursorPositionEventItem;
            EventSystem::CursorEnterEventItem cursorEnterEventItem;
            EventSystem::ScrollEventItem scrollEventItem;
            EventSystem::ResolutionEventItem resolutionEventItem;

            static InputStatus keyboardButtons[L2DE_KEYBOARD_BUTTON_NUMBER];
            static Cursor cursor;

            /// <summary>
            /// It is multiplied by timeScale.
            /// Before the first frame, it is 0.0f.
            /// </summary>
            static float deltaTime;

            void UpdateEvents();
            /// <summary>
            /// The glfwPollEvents doesn't refresh the data on every frame.
            /// That's why this function update the InputStatus::KEY_DOWN to InputStatus::KEY_HOLD.
            /// </summary>
            void FixKeyboardButtons();
            /// <summary>
            /// The glfwPollEvents doesn't have InputStatus::KEY_HOLD for Mouse buttons.
            /// Moreover it doesn't refresh the scroll values to 0.0f.
            /// So this function do it.
            /// </summary>
            void FixCursor();
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

            void RefreshKeyboard(int key, int scancode, int action, int mode) override;
            void RefreshMouseButton(int button, int action, int mods) override;
            void RefreshCursorPosition(double xpos, double ypos) override;
            void RefreshCursorInWindows(bool entered) override;
            void RefreshScroll(double xoffset, double yoffset) override;

            /// <summary>
            /// If this function is override, it must call the Game::RefreshResolution(const Resolution& resolution) in the first line.
            /// </summary>
            virtual void RefreshResolution(const Render::Resolution& resolution) override;

            static float GetDeltaTime()
            {
                return deltaTime;
            }

            static InputStatus GetKeyboardButtonStatus(int key)
            {
                return keyboardButtons[key];
            }

            static InputStatus GetMouseButtonStatus(int key)
            {
                return cursor.mouseButtons[key];
            }

            static glm::vec2 GetCursorPosition()
            {
                return cursor.position;
            }

            static bool IsCursorInWindow()
            {
                return cursor.isInWindow;
            }

            static glm::vec2 GetScroll()
            {
                return cursor.scroll;
            }
        };
    }
}