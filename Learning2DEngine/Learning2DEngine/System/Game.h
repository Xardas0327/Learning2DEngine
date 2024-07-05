#pragma once

#include <glm/glm.hpp>

#include "InputStatus.h"
#include "../Render/RenderManager.h"



namespace Learning2DEngine
{
    namespace System
    {
#define INPUT_KEY_SIZE 1024
#define TIME_SCALE_DEFAULT 1.0f

        /*
            The Function order in the Run() (in a frame):
            Calculate deltaTime
            Refresh Keyboard and Mouse events
            virtual Update()
            Clear Screen to default color
            virtual Render()
            Update Screen
        */
        class Game
        {
        private:
            float lastFrame;
            float timeScale;

            /// <summary>
            /// It is multiplied by timeScale.
            /// Before the Game::Run(), it is 0.0f.
            /// </summary>
            static float deltaTime;

            void UpdateKeyboardMouseEvents();
            static void CallbackRefreshKeyboardMouse(int key, int scancode, int action, int mode);
            /// <summary>
            /// The glfwPollEvents doesn't refresh the data on every frame.
            /// That's why this function update the InputStatus::KEY_DOWN to InputStatus::KEY_HOLD.
            /// </summary>
            void FixKeyboardMouse();
        protected:
            static InputStatus inputKeys[INPUT_KEY_SIZE];

            /// <summary>
            /// It has to be initialized.
            /// It is glm::mat4(0.0f) by default.
            /// </summary>
            static glm::mat4 cameraProjection;

            virtual void Update() {};
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
            /// The Blend is enable by default with Source: SRC_ALPHA, Destination: ONE_MINUS_SRC_ALPHA,
            /// because it is used for some features.
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

            static float GetDeltaTime()
            {
                return deltaTime;
            }

            inline static glm::mat4 GetCameraProjection()
            {
                return cameraProjection;
            }
        };
    }
}