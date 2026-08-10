#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "InputStatus.h"

namespace Learning2DEngine
{
	namespace System
	{
		struct Gamepad
		{
			static constexpr int GamepadButtonNumber = GLFW_GAMEPAD_BUTTON_LAST + 1;
			static constexpr int GamepadAxisNumber = GLFW_GAMEPAD_AXIS_LAST + 1;

			InputStatus gamepadButtons[Gamepad::GamepadButtonNumber];
			float gamepadAxes[Gamepad::GamepadAxisNumber];

			Gamepad()
				: gamepadButtons{ InputStatus::KEY_UP }, gamepadAxes{ 0.0f }
			{}

			void Reset()
			{
				for (int i = 0; i < GamepadButtonNumber; ++i)
					gamepadButtons[i] = InputStatus::KEY_UP;

				for (int i = 0; i < GamepadAxisNumber; ++i)
					gamepadAxes[i] = 0.0f;
			}
		};
	}
}
