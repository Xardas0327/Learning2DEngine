#include "RenderManager.h"

#include <exception>

namespace Learning2DEngine
{
    using namespace System;

    namespace Render
    {
        RenderManager::RenderManager() :
            window(nullptr), screenWidth(0), screenHeight(0),
            keyboardEventHandler(), framebufferSizeEventHandler()
        {

        }

        void RenderManager::Init(int majorVersion, int minorVersion, int screenWidth, int screenHeight, const char* title)
        {
            this->screenWidth = screenWidth;
            this->screenHeight = screenHeight;

            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);
            if (window == NULL)
            {
                glfwTerminate();
                throw std::exception("ERROR::GLFW: Failed to create GLFW window");
            }
            glfwMakeContextCurrent(window);
            glfwSetKeyCallback(window, CallbackUpdateKeyboardMouse);
            glfwSetFramebufferSizeCallback(window, CallbackUpdateFramebufferSize);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                throw std::exception("ERROR::GLAD: Failed to initialize GLAD");
            }

            glViewport(0, 0, screenWidth, screenHeight);
        }

        void RenderManager::Terminate()
        {
            glfwTerminate();
        }

        void RenderManager::CloseWindow()
        {
            glfwSetWindowShouldClose(window, true);
        }

        bool RenderManager::IsWindowClosed()
        {
            return glfwWindowShouldClose(window);
        }

        void RenderManager::SetVSync(bool value)
        {
            glfwSwapInterval(value);
        }

        void RenderManager::UpdateScreen()
        {
            glfwSwapBuffers(window);
        }

        void RenderManager::ClearScreen()
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void RenderManager::CallbackUpdateKeyboardMouse(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            RenderManager::GetInstance().UpdateKeyboardMouse(window, key, scancode, action, mode);
        }

        void RenderManager::UpdateKeyboardMouse(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            keyboardEventHandler.Invoke(key, scancode, action, mode);
        }

        void RenderManager::CallbackUpdateFramebufferSize(GLFWwindow* window, int width, int height)
        {
            RenderManager::GetInstance().UpdateFramebufferSize(window, width, height);
        }

        void RenderManager::UpdateFramebufferSize(GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
            screenWidth = screenWidth;
            screenHeight = screenHeight;
            framebufferSizeEventHandler.Invoke(width, height);
        }

        void RenderManager::AddKeyboardEvent(const EventHandler<int, int, int, int>::EventFunction func)
        {
            keyboardEventHandler.Add(func);
        }

        void RenderManager::RemoveKeyboardEvent(const EventHandler<int, int, int, int>::EventFunction func)
        {
            keyboardEventHandler.Remove(func);
        }

        void RenderManager::AddFramebufferSizeEvent(const System::EventHandler<int, int>::EventFunction func)
        {
            framebufferSizeEventHandler.Add(func);
        }

        void RenderManager::RemoveFramebufferSizeEvent(const System::EventHandler<int, int>::EventFunction func)
        {
            framebufferSizeEventHandler.Remove(func);
        }
    }
}