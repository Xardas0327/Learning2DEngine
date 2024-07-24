#pragma once

namespace Learning2DEngine
{
	namespace System
	{
		class IKeyboardMouseRefresher
		{
		public:
			virtual ~IKeyboardMouseRefresher() {}

			virtual void RefreshKeyboardMouse(int key, int scancode, int action, int mode) = 0;
		};
	}
}