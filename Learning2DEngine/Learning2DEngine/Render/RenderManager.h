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
		// First is the source factor.
		// Second is the destination factor.
		typedef std::pair<unsigned int, unsigned int> BlendFuncFactor;

		class RenderManager : public virtual System::Singleton<RenderManager>
		{
			friend class System::Singleton<RenderManager>;
		private:
			GLFWwindow* window;
			Resolution resolution;
			glm::vec4 clearColor;
			System::EventHandler<int, int, int, int> keyboardEventHandler;
			System::EventHandler<Resolution> framebufferSizeEventHandler;
			bool isBlendActive;
			BlendFuncFactor blendFuncFactor;


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
			inline Resolution GetResolution() 
			{
				return resolution;
			}

			void SetClearColort(float r, float g, float b, float a);
			inline glm::vec4 GetClearColort()
			{
				return clearColor;
			}

			inline void EnableBlend()
			{
				if (!isBlendActive)
				{
					isBlendActive = true;
					glEnable(GL_BLEND);
				}
			}

			inline void DisableBlend()
			{
				if (isBlendActive)
				{
					isBlendActive = false;
					glDisable(GL_BLEND);
				}
			}

			inline bool IsBlendActive()
			{
				return isBlendActive;
			}

			inline void SetBlendFunc(unsigned int sourceFactor, unsigned int destinationFactor)
			{
				BlendFuncFactor newBlendFuncFactor(sourceFactor, destinationFactor);
				if (this->blendFuncFactor != newBlendFuncFactor)
				{
					this->blendFuncFactor = newBlendFuncFactor;
					glBlendFunc(this->blendFuncFactor.first, this->blendFuncFactor.second);
				}
			}

			inline void SetBlendFunc(BlendFuncFactor blendFuncFactor)
			{
				if (this->blendFuncFactor != blendFuncFactor)
				{
					this->blendFuncFactor = blendFuncFactor;
					glBlendFunc(this->blendFuncFactor.first, this->blendFuncFactor.second);
				}
			}

			inline BlendFuncFactor GetBlendFunc()
			{
				return this->blendFuncFactor;
			}

			void AddKeyboardEvent(const System::EventHandler<int, int, int, int>::EventFunction func);
			void RemoveKeyboardEvent(const System::EventHandler<int, int, int, int>::EventFunction func);
			void AddFramebufferSizeEvent(const System::EventHandler<Resolution>::EventFunction func);
			void RemoveFramebufferSizeEvent(const System::EventHandler<Resolution>::EventFunction func);
		};
	}
}