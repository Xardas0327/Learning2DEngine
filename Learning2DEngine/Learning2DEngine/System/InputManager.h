#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Cursor.h"
#include "Gamepad.h"
#include "ICursorRefresher.h"
#include "IKeyboardRefresher.h"
#include "InputStatus.h"
#include "Singleton.h"
#include "../EventSystem/KeyboardEventItem.h"
#include "../EventSystem/MouseButtonEventItem.h"
#include "../EventSystem/CursorPositionEventItem.h"
#include "../EventSystem/CursorEnterEventItem.h"
#include "../EventSystem/ScrollEventItem.h"

namespace Learning2DEngine
{

    namespace System
    {
        class InputManager final : public Singleton<InputManager>,
                                    private ICursorRefresher,
                                    private IKeyboardRefresher
        {
            friend class Singleton<InputManager>;
            friend class Game;
        private:
            static constexpr int KeyboardButtonNumber = GLFW_KEY_LAST + 1;
            static constexpr int GamepadNumber = GLFW_JOYSTICK_LAST + 1;

            InputStatus keyboardButtons[InputManager::KeyboardButtonNumber];
            Cursor cursor;
            Gamepad gamepads[InputManager::GamepadNumber];

            EventSystem::KeyboardEventItem keyboardEventItem;
            EventSystem::MouseButtonEventItem mouseButtonEventItem;
            EventSystem::CursorPositionEventItem cursorPositionEventItem;
            EventSystem::CursorEnterEventItem cursorEnterEventItem;
            EventSystem::ScrollEventItem scrollEventItem;

            InputManager();

			void Init();
			void Terminate();

            void UpdateEvents();
            /// <summary>
            /// The glfwPollEvents doesn't refresh the data on every frame.
            /// That's why this function update the InputStatus::KEY_DOWN to InputStatus::KEY_HOLD.
            /// </summary>
            void FixKeyboardButtons();
            /// <summary>
            /// The glfwPollEvents doesn't have InputStatus::KEY_HOLD for Mouse buttons.
            /// Moreover it doesn't refresh the scroll values to 0.0f.
            /// So this function do it.
            /// </summary>
            void FixCursor();
            /// <summary>
            /// The GLFW doesn't have InputStatus::KEY_HOLD for Gamepad buttons.
            /// Moreover it has to ask all connected gamepad states one by one.
            /// </summary>
			void FixGamepad();

            void RefreshKeyboard(int key, int scancode, int action, int mode) override;
            void RefreshMouseButton(int button, int action, int mods) override;
            void RefreshCursorPosition(double xpos, double ypos) override;
            void RefreshCursorInWindows(bool entered) override;
            void RefreshScroll(double xoffset, double yoffset) override;
        public:
			~InputManager() = default;

            inline InputStatus GetKeyboardButtonStatus(int key) const
            {
                return keyboardButtons[key];
            }

			inline const Cursor& GetCursor() const
			{
				return cursor;
			}

            inline InputStatus GetMouseButtonStatus(int key) const
            {
                return cursor.mouseButtons[key];
            }

            inline glm::vec2 GetCursorPosition() const
            {
                return cursor.position;
            }

            inline bool IsCursorInWindow() const
            {
                return cursor.isInWindow;
            }

            inline glm::vec2 GetCursorScroll() const
            {
                return cursor.scroll;
            }

			inline bool IsGamepadConnected(int joystickId) const
			{
				return glfwJoystickPresent(joystickId);
			}

			inline const char* GetGamepadName(int joystickId) const
			{
				return glfwGetGamepadName(joystickId);
			}

			inline const Gamepad& GetGamepad(int joystickId) const
			{
				return gamepads[joystickId];
			}

			inline InputStatus GetGamepadButtonStatus(int joystickId, int button) const
			{
				return gamepads[joystickId].gamepadButtons[button];
			}

            inline float GetGamepadAxisStatus(int joystickId, int axis) const
            {
				return gamepads[joystickId].gamepadAxes[axis];
            }
        };
    }
}