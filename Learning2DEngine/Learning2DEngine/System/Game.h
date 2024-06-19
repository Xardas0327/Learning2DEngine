#pragma once

#include "InputStatus.h"
#include "../Render/RenderManager.h"

/*
* The Function order in the Run() (in a frame):
*    Calculate deltaTime
*    Refresh Keyboard and Mouse events
* 1. Update(float deltaTime)
*    Clear Screen to default color
* 2. Render()
*    Update Screen
* 
* Note: those functions, which start with number, are virtual function
*/

namespace Learning2DEngine
{
    namespace System
    {
#define INPUT_KEY_SIZE 1024
#define TIME_SCALE_DEFAULT 1.0f

        /// <summary>
        /// Function order in the Run()
        /// 1. Update
        /// 2. Render
        /// </summary>
        class Game
        {
        private:
            float deltaTime;
            float lastFrame;
            float timeScale;

            void UpdateKeyboardMouseEvents();
            static void CallbackRefreshKeyboardMouse(int key, int scancode, int action, int mode);
            /// <summary>
            /// The glfwPollEvents doesn't refresh the data on every frame.
            /// That's why this function update the InputStatus::KEY_DOWN to InputStatus::KEY_HOLD.
            /// </summary>
            void FixKeyboardMouse();
        protected:
            static InputStatus inputKeys[INPUT_KEY_SIZE];

            virtual void Update(float deltaTime) {};
            virtual void Render() {};
        public:
            Game();
            virtual ~Game();

            /// <summary>
            /// It will initialize the RenderManager.
            /// After that, the Init() will be called.
            /// </summary>
            void InitWithRender(int majorRenderVersion, int minorRenderVersion, Render::Resolution resolution, const char* title);
            /// <summary>
            /// The RenderManager have to be initialized before this.
            /// If this function is override, it must call the Game::Init() in the first line.
            /// </summary>
            virtual void Init();
            /// <summary>
            /// It calls the RenderManager::Terminate()
            /// If this function is override, it must call the Game::Terminate() in the last line.
            /// </summary>
            virtual void Terminate();
            void Run();

            inline void ResetTimeScale() 
            {
                timeScale = TIME_SCALE_DEFAULT;
            }

            inline void SetTimeScale(float value)
            {
                timeScale = value;
            }

            inline float GetTimeScale()
            {
                return timeScale;
            }
        };
    }
}