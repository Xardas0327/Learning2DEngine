#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Resolution.h"
#include "../System/Singleton.h"
#include "../System/EventHandler.h"

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
			System::EventHandler<int, int, int, int> keyboardEventHandler;
			System::EventHandler<Resolution> framebufferSizeEventHandler;

			RenderManager();

			static void CallbackUpdateKeyboardMouse(GLFWwindow* window, int key, int scancode, int action, int mode);
			void UpdateKeyboardMouse(GLFWwindow* window, int key, int scancode, int action, int mode);
			static void CallbackUpdateFramebufferSize(GLFWwindow* window, int width, int height);
			void UpdateFramebufferSize(GLFWwindow* window, int width, int height);
		public:
			void Init(int majorVersion, int minorVersion, Resolution resolution, const char* title);
			void Terminate();
			void CloseWindow();
			bool IsWindowClosed();
			void SetVSync(bool value);
			void UpdateScreen();
			void ClearScreen();
			inline Resolution GetResolution() { return resolution; }
			void SetClearColort(float r, float g, float b, float a);
			inline glm::vec4 GetClearColort() { return clearColor; }

			void AddKeyboardEvent(const System::EventHandler<int, int, int, int>::EventFunction func);
			void RemoveKeyboardEvent(const System::EventHandler<int, int, int, int>::EventFunction func);
			void AddFramebufferSizeEvent(const System::EventHandler<Resolution>::EventFunction func);
			void RemoveFramebufferSizeEvent(const System::EventHandler<Resolution>::EventFunction func);
		};
	}
}