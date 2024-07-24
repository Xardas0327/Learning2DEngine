#pragma once

#include "EventItem.h"
#include "../System/IKeyboardMouseRefresher.h"

namespace Learning2DEngine
{
	namespace EventSystem
	{
		class KeyboardMouseEventItem final : public virtual EventItem<int, int, int, int>
		{
		private:
			System::IKeyboardMouseRefresher* refresher;
		public:
			KeyboardMouseEventItem(System::IKeyboardMouseRefresher* refresher)
				: refresher(refresher)
			{

			}

			void Call(int key, int scancode, int action, int mode) override
			{
				refresher->RefreshKeyboardMouse(key, scancode, action, mode);
			}
		};
	}
}