#include "RenderManager.h"

#include "../DebugTool/Log.h"

namespace Learning2DEngine
{
    using namespace EventSystem;

    namespace Render
    {
        RenderManager::RenderManager() :
            window(nullptr), resolution(0, 0), clearColor(0.0f, 0.0f, 0.0f, 1.0f),
            keyboardEventHandler(), framebufferSizeEventHandler(),
            isBlendActive(false), blendFuncFactor(GL_ONE, GL_ZERO)
        {

        }

        void RenderManager::Init(int majorVersion, int minorVersion, Resolution resolution, const char* title, bool resizableWindows)
        {
            this->resolution = resolution;

            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
            if(!resizableWindows)
                glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            window = glfwCreateWindow(resolution.GetWidth(), resolution.GetHeight(), title, nullptr, nullptr);
            if (window == NULL)
            {
                glfwTerminate();
                L2DE_LOG_ERROR("GLFW: Failed to create GLFW window");
                return;
            }
            glfwMakeContextCurrent(window);
            glfwSetKeyCallback(window, RenderManager::CallbackUpdateKeyboard);
            glfwSetFramebufferSizeCallback(window, RenderManager::CallbackUpdateFramebufferSize);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                L2DE_LOG_ERROR("GLAD: Failed to initialize GLAD");
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

        void RenderManager::UpdateWindow()
        {
            glfwSwapBuffers(window);
        }

        void RenderManager::ClearWindow()
        {
            glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void RenderManager::SetClearColor(float r, float g, float b, float a)
        {
            clearColor = glm::vec4(r, g, b, a);
        }

        void RenderManager::EnableBlend()
        {
            if (!isBlendActive)
            {
                isBlendActive = true;
                glEnable(GL_BLEND);
            }
        }

        void RenderManager::DisableBlend()
        {
            if (isBlendActive)
            {
                isBlendActive = false;
                glDisable(GL_BLEND);
            }
        }

        void RenderManager::SetBlendFunc(unsigned int sourceFactor, unsigned int destinationFactor)
        {
            BlendFuncFactor newBlendFuncFactor(sourceFactor, destinationFactor);
            if (blendFuncFactor != newBlendFuncFactor)
            {
                blendFuncFactor = newBlendFuncFactor;
                glBlendFunc(blendFuncFactor.first, blendFuncFactor.second);
            }
        }

        void RenderManager::SetBlendFunc(BlendFuncFactor blendFuncFactor)
        {
            if (this->blendFuncFactor != blendFuncFactor)
            {
                this->blendFuncFactor = blendFuncFactor;
                glBlendFunc(this->blendFuncFactor.first, this->blendFuncFactor.second);
            }
        }

        void RenderManager::CallbackUpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            RenderManager::GetInstance().UpdateKeyboard(window, key, scancode, action, mode);
        }

        void RenderManager::UpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
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

        void RenderManager::AddKeyboardEvent(EventItem<int, int, int, int>* evenItem)
        {
            keyboardEventHandler.Add(evenItem);
        }

        void RenderManager::RemoveKeyboardEvent(EventItem<int, int, int, int>* evenItem)
        {
            keyboardEventHandler.Remove(evenItem);
        }

        void RenderManager::AddFramebufferSizeEvent(EventItem<Resolution>* evenItem)
        {
            framebufferSizeEventHandler.Add(evenItem);
        }

        void RenderManager::RemoveFramebufferSizeEvent(EventItem<Resolution>* evenItem)
        {
            framebufferSizeEventHandler.Remove(evenItem);
        }
    }
}