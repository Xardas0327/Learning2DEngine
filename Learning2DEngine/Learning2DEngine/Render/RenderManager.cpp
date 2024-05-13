#include "RenderManager.h"

#include <exception>

namespace Learning2DEngine
{
    using namespace System;

    namespace Render
    {
        RenderManager::RenderManager() :
            window(nullptr), keyboardEventHandler(), updateEventHandler(), renderEventHandler()
        {

        }

        void RenderManager::Init(int majorVersion, int minorVersion, int screenWidth, int screenHeight, const char* title)
        {
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
            glfwSetKeyCallback(window, CallbackKeyboardWrapper);
            glfwSetFramebufferSizeCallback(window, CallbackFramebufferSizeWrapper);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                throw std::exception("ERROR::GLAD: Failed to initialize GLAD");
            }

            glViewport(0, 0, screenWidth, screenHeight);
        }

        void RenderManager::Run()
        {
            float deltaTime = 0.0f;
            float lastFrame = 0.0f;
            while (!glfwWindowShouldClose(window))
            {
                // Calc deltaTime
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;
                glfwPollEvents();

                updateEventHandler.Invoke(deltaTime);


                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                renderEventHandler.Invoke();

                glfwSwapBuffers(window);
            }
        }

        void RenderManager::CloseWindow()
        {
            glfwSetWindowShouldClose(window, true);
        }

        void RenderManager::Terminate()
        {
            glfwTerminate();
        }

        void RenderManager::CallbackKeyboardWrapper(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            RenderManager::GetInstance().CallbackKeyboard(window, key, scancode, action, mode);
        }

        void RenderManager::CallbackKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            keyboardEventHandler.Invoke(window, key, scancode, action, mode);
        }

        void RenderManager::CallbackFramebufferSizeWrapper(GLFWwindow* window, int width, int height)
        {
            RenderManager::GetInstance().CallbackFramebufferSize(window, width, height);
        }

        void RenderManager::CallbackFramebufferSize(GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }

        void RenderManager::AddKeyboardEvent(const EventHandler<GLFWwindow*, int, int, int, int>::EventFunction func)
        {
            keyboardEventHandler.Add(func);
        }

        void RenderManager::RemoveKeyboardEvent(const EventHandler<GLFWwindow*, int, int, int, int>::EventFunction func)
        {
            keyboardEventHandler.Remove(func);
        }

        void RenderManager::AddUpdateEvent(const EventHandler<float>::EventFunction func)
        {
            updateEventHandler.Add(func);
        }

        void RenderManager::RemoveUpdateEvent(const EventHandler<float>::EventFunction func)
        {
            updateEventHandler.Remove(func);
        }

        void RenderManager::AddRenderEvent(const EventHandler<>::EventFunction func)
        {
            renderEventHandler.Add(func);
        }

        void RenderManager::RemoveRenderEvent(const EventHandler<>::EventFunction func)
        {
            renderEventHandler.Remove(func);
        }
    }
}