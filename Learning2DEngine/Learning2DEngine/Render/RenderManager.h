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
			void Init(int majorVersion, int minorVersion, int screenWith, int screenHeight, const char* title);
			void Run();
			void CloseWindow();
			void Terminate();

			void AddKeyboardEvent(const System::EventHandler<GLFWwindow*, int, int, int, int>::EventFunction func);
			void RemoveKeyboardEvent(const System::EventHandler<GLFWwindow*, int, int, int, int>::EventFunction func);
			void AddUpdateEvent(const System::EventHandler<float>::EventFunction func);
			void RemoveUpdateEvent(const System::EventHandler<float>::EventFunction func);
			void AddRenderEvent(const System::EventHandler<>::EventFunction func);
			void RemoveRenderEvent(const System::EventHandler<>::EventFunction func);
		};
	}
}