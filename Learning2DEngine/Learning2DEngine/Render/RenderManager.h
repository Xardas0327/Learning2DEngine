#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
			System::EventHandler<GLFWwindow*, int, int, int, int> keyboardEventHandler;
			// float is the delta time
			System::EventHandler<float> updateEventHandler;
			System::EventHandler<> renderEventHandler;

			RenderManager();

			// Callbacks

			static void CallbackKeyboardWrapper(GLFWwindow* window, int key, int scancode, int action, int mode);
			void CallbackKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode);
			static void CallbackFramebufferSizeWrapper(GLFWwindow* window, int width, int height);
			void CallbackFramebufferSize(GLFWwindow* window, int width, int height);
		public:
			void Init(const int& majorVersion, const int& minorVersion, const int& screenWith, const int& screenHeight, const char* title);
			void Run();
			void CloseWindow();
			void Terminate();

			void AddKeyboardEvent(System::EventHandler<GLFWwindow*, int, int, int, int>::EventFunction func);
			void RemoveKeyboardEvent(System::EventHandler<GLFWwindow*, int, int, int, int>::EventFunction func);
			void AddUpdateEvent(System::EventHandler<float>::EventFunction func);
			void RemoveUpdateEvent(System::EventHandler<float>::EventFunction func);
			void AddRenderEvent(System::EventHandler<>::EventFunction func);
			void RemoveRenderEvent(System::EventHandler<>::EventFunction func);
		};
	}
}