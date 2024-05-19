#pragma once

#include "InputStatus.h"
#include "../Render/RenderManager.h"

/*
* The Function order in the Run() (in a frame):
*    Calculate deltaTime
*    Refresh Keyboard and Mouse events
* 1. ProcessInput(float deltaTime)
* 2. Update(float deltaTime)
*    Clear Screen to default color
* 3. Render()
*    Update Screen
* 
* Note: those functions, which start with number, are virtual function
*/

namespace Learning2DEngine
{
    namespace System
    {
#define INPUT_KEY_SIZE 1024
        /// <summary>
        /// Function order in the Run()
        /// 1. ProcessInput
        /// 2. Update
        /// 3. Render
        /// </summary>
        class Game
        {
        private:
            float deltaTime, lastFrame;

            static void CallbackRefreshKeyboard(int key, int scancode, int action, int mode);
        protected:
            static InputStatus inputKeys[INPUT_KEY_SIZE];

            virtual void ProcessInput(float deltaTime) {};
            virtual void Update(float deltaTime) {};
            virtual void Render() {};
        public:
            Game();
            virtual ~Game();

            /// <summary>
            /// It will initialize the RenderManager.
            /// After that, the Init() will be called.
            /// </summary>
            void Init(int majorRenderVersion, int minorRenderVersion, int screenWidth, int screenHeight, const char* title);
            /// <summary>
            /// The RenderManager have to be initialized before this.
            /// </summary>
            void Init();
            void Terminate();
            void Run();
        };

        InputStatus Game::inputKeys[INPUT_KEY_SIZE] = { InputStatus::KEY_UP };
    }
}