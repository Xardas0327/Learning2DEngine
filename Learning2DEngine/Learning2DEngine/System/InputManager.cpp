#include "InputManager.h"

#include "../DebugTool/Log.h"
#include "../Render/RenderManager.h"

namespace Learning2DEngine
{
    using namespace Render;

    namespace System
    {
        InputManager::InputManager()
			: keyboardButtons{ InputStatus::KEY_UP }, cursor(), gamepads(),
            keyboardEventItem(this), mouseButtonEventItem(this), cursorPositionEventItem(this),
			cursorEnterEventItem(this), scrollEventItem(this)
        {
        }

        void InputManager::Init()
        {
			for (int i = 0; i < InputManager::KeyboardButtonNumber; ++i)
				keyboardButtons[i] = InputStatus::KEY_UP;

			cursor.Reset();

			for (int i = 0; i < InputManager::GamepadNumber; ++i)
                gamepads[i].Reset();

            auto& renderManager = RenderManager::GetInstance();
            renderManager.AddKeyboardEvent(&keyboardEventItem);
            renderManager.AddMouseButtonEvent(&mouseButtonEventItem);
            renderManager.AddCursorPositonEvent(&cursorPositionEventItem);
            renderManager.AddCursorEnterEvent(&cursorEnterEventItem);
            renderManager.AddScrollEvent(&scrollEventItem);
        }

        void InputManager::Terminate()
        {
            auto& renderManager = RenderManager::GetInstance();
            renderManager.RemoveKeyboardEvent(&keyboardEventItem);
            renderManager.RemoveMouseButtonEvent(&mouseButtonEventItem);
            renderManager.RemoveCursorPositonEvent(&cursorPositionEventItem);
            renderManager.RemoveCursorEnterEvent(&cursorEnterEventItem);
            renderManager.RemoveScrollEvent(&scrollEventItem);
        }

        void InputManager::UpdateEvents()
        {
            FixKeyboardButtons();
            FixCursor();
            glfwPollEvents();

            FixGamepad();
        }

        void InputManager::FixKeyboardButtons()
        {
            for (int i = 0; i < InputManager::KeyboardButtonNumber; ++i)
            {
                if (keyboardButtons[i] == InputStatus::KEY_DOWN)
                    keyboardButtons[i] = InputStatus::KEY_HOLD;
            }
        }

        void InputManager::FixCursor()
        {
            for (int i = 0; i < Cursor::MouseButtonNumber; ++i)
            {
                if (cursor.mouseButtons[i] == InputStatus::KEY_DOWN)
                    cursor.mouseButtons[i] = InputStatus::KEY_HOLD;
            }

            cursor.scroll.x = 0.0f;
            cursor.scroll.y = 0.0f;
        }

        void InputManager::FixGamepad()
        {
			for (int i = 0; i < InputManager::GamepadNumber; ++i)
			{
				if (glfwJoystickPresent(i))
				{
					GLFWgamepadstate state;
					if (glfwGetGamepadState(i, &state))
					{
						for (int j = 0; j < Gamepad::GamepadButtonNumber; ++j)
						{
							switch (state.buttons[j])
							{
							case GLFW_RELEASE:
								gamepads[i].gamepadButtons[j] = InputStatus::KEY_UP;
								break;
							case GLFW_PRESS:
                                if(gamepads[i].gamepadButtons[j] == InputStatus::KEY_UP)
								    gamepads[i].gamepadButtons[j] = InputStatus::KEY_DOWN;
                                else
                                    gamepads[i].gamepadButtons[j] = InputStatus::KEY_HOLD;
								break;
							default:
								L2DE_LOG_ERROR("InputManager: Unknown gamepad button state: " + std::to_string(state.buttons[j]))
									break;
							}
						}

						for (int j = 0; j < Gamepad::GamepadAxisNumber; ++j)
						{
							gamepads[i].gamepadAxes[j] = state.axes[j];
						}
					}
				}
				else
                    gamepads[i].Reset();
			}
        }

        void InputManager::RefreshKeyboard(int key, int scancode, int action, int mode)
        {
            if (key > GLFW_KEY_UNKNOWN && key < InputManager::KeyboardButtonNumber)
            {
                switch (action)
                {
                case GLFW_RELEASE:
                    keyboardButtons[key] = InputStatus::KEY_UP;
                    break;
                case GLFW_PRESS:
                    keyboardButtons[key] = InputStatus::KEY_DOWN;
                    break;
                case GLFW_REPEAT:
                    keyboardButtons[key] = InputStatus::KEY_HOLD;
                    break;
                default:
                    L2DE_LOG_ERROR("InputManager: Unknown input action: " + std::to_string(action))
                    break;
                }
            }
        }

        void InputManager::RefreshMouseButton(int button, int action, int mods)
        {
            if (button >= 0 && button < Cursor::MouseButtonNumber)
            {
                switch (action)
                {
                case GLFW_RELEASE:
                    cursor.mouseButtons[button] = InputStatus::KEY_UP;
                    break;
                case GLFW_PRESS:
                    cursor.mouseButtons[button] = InputStatus::KEY_DOWN;
                    break;
                case GLFW_REPEAT:
                    cursor.mouseButtons[button] = InputStatus::KEY_HOLD;
                    break;
                default:
                    L2DE_LOG_ERROR("InputManager: Unknown input action: " + std::to_string(action))
                    break;
                }
            }
        }

        void InputManager::RefreshCursorPosition(double xpos, double ypos)
        {
            cursor.position.x = static_cast<float>(xpos);
            cursor.position.y = static_cast<float>(ypos);
        }

        void InputManager::RefreshCursorInWindows(bool entered)
        {
            cursor.isInWindow = entered;
        }

        void InputManager::RefreshScroll(double xoffset, double yoffset)
        {
            cursor.scroll.x = static_cast<float>(xoffset);
            cursor.scroll.y = static_cast<float>(yoffset);
        }
    }
}