#include "RenderManager.h"

#include "../System/Log.h"

namespace Learning2DEngine
{
    using namespace System;

    namespace Render
    {
        RenderManager::RenderManager() :
            window(nullptr), resolution(0, 0), clearColor(0.0f, 0.0f, 0.0f, 1.0f),
            keyboardEventHandler(), framebufferSizeEventHandler()
        {

        }

        void RenderManager::Init(int majorVersion, int minorVersion, Resolution resolution, const char* title)
        {
            this->resolution = resolution;

            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            window = glfwCreateWindow(resolution.GetWidth(), resolution.GetHeight(), title, nullptr, nullptr);
            if (window == NULL)
            {
                glfwTerminate();
                Log::Error("GLFW: Failed to create GLFW window");
                return;
            }
            glfwMakeContextCurrent(window);
            glfwSetKeyCallback(window, CallbackUpdateKeyboardMouse);
            glfwSetFramebufferSizeCallback(window, CallbackUpdateFramebufferSize);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                Log::Error("GLAD: Failed to initialize GLAD");
                return;
            }

            glViewport(0, 0, resolution.GetWidth(), resolution.GetHeight());
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
            glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void RenderManager::SetClearColort(float r, float g, float b, float a)
        {
            clearColor = glm::vec4(r, g, b, a);
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
            resolution = Resolution(width, height);
            framebufferSizeEventHandler.Invoke(resolution);
        }

        void RenderManager::AddKeyboardEvent(const EventHandler<int, int, int, int>::EventFunction func)
        {
            keyboardEventHandler.Add(func);
        }

        void RenderManager::RemoveKeyboardEvent(const EventHandler<int, int, int, int>::EventFunction func)
        {
            keyboardEventHandler.Remove(func);
        }

        void RenderManager::AddFramebufferSizeEvent(const System::EventHandler<Resolution>::EventFunction func)
        {
            framebufferSizeEventHandler.Add(func);
        }

        void RenderManager::RemoveFramebufferSizeEvent(const System::EventHandler<Resolution>::EventFunction func)
        {
            framebufferSizeEventHandler.Remove(func);
        }
    }
}