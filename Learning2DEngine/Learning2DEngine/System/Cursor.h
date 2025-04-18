#pragma once

#include <glm/glm.hpp>
#include "InputStatus.h"

namespace Learning2DEngine
{
	namespace System
	{
#define L2DE_MOUSE_BUTTON_NUMBER 8

		struct Cursor {
			InputStatus mouseButtons[L2DE_MOUSE_BUTTON_NUMBER];
			glm::vec2 position;
			glm::vec2 scroll;
			bool isInWindow;
		};
	}
}