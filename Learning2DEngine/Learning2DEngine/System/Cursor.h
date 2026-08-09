#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "InputStatus.h"

namespace Learning2DEngine
{
	namespace System
	{
		struct Cursor
		{
			static constexpr int MouseButtonNumber = GLFW_MOUSE_BUTTON_LAST + 1;

			InputStatus mouseButtons[Cursor::MouseButtonNumber];
			glm::vec2 position;
			glm::vec2 scroll;
			bool isInWindow;

			Cursor()
				: mouseButtons{ InputStatus::KEY_UP }, position(0.0f, 0.0f), scroll(0.0f, 0.0f), isInWindow(false)
			{}

			void Reset()
			{
				for (int i = 0; i < MouseButtonNumber; ++i)
					mouseButtons[i] = InputStatus::KEY_UP;

				position = glm::vec2(0.0f, 0.0f);
				scroll = glm::vec2(0.0f, 0.0f);
				isInWindow = false;
			}
		};
	}
}