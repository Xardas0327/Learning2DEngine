#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Resolution.h"
#include "Blend.h"
#include "../System/Singleton.h"
#include "../EventSystem/EventHandler.h"
#include "../EventSystem/EventItem.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class RenderManager : public virtual System::Singleton<RenderManager>
		{
			friend class System::Singleton<RenderManager>;
		private:
			GLFWwindow* window;
			Resolution resolution;
			glm::vec4 clearColor;
			EventSystem::EventHandler<int, int, int, int> keyboardEventHandler;
			EventSystem::EventHandler<Resolution> framebufferSizeEventHandler;
			bool isBlendActive;
			BlendFuncFactor blendFuncFactor;

			RenderManager();

			static void CallbackUpdateKeyboardMouse(GLFWwindow* window, int key, int scancode, int action, int mode);
			void UpdateKeyboardMouse(GLFWwindow* window, int key, int scancode, int action, int mode);
			static void CallbackUpdateFramebufferSize(GLFWwindow* window, int width, int height);
			void UpdateFramebufferSize(GLFWwindow* window, int width, int height);
		public:
			void Init(int majorVersion, int minorVersion, Resolution resolution, const char* title, bool resizableWindows = true);
			void Terminate();
			void CloseWindow();
			bool IsWindowClosed();
			void SetVSync(bool value);
			void UpdateWindow();
			void ClearWindow();
			inline Resolution GetResolution() 
			{
				return resolution;
			}

			void SetClearColort(float r, float g, float b, float a);
			inline glm::vec4 GetClearColort()
			{
				return clearColor;
			}

			void EnableBlend();
			void DisableBlend();
			inline bool IsBlendActive()
			{
				return isBlendActive;
			}

			void SetBlendFunc(unsigned int sourceFactor, unsigned int destinationFactor);
			void SetBlendFunc(BlendFuncFactor blendFuncFactor);
			inline BlendFuncFactor GetBlendFunc()
			{
				return blendFuncFactor;
			}

			void AddKeyboardEvent(EventSystem::EventItem<int, int, int, int>* evenItem);
			void RemoveKeyboardEvent(EventSystem::EventItem<int, int, int, int>* evenItem);
			void AddFramebufferSizeEvent(EventSystem::EventItem<Resolution>* evenItem);
			void RemoveFramebufferSizeEvent(EventSystem::EventItem<Resolution>* evenItem);
		};
	}
}