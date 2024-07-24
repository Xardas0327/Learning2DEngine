#pragma once

#include "EventItem.h"
#include "IKeyboardMouseRefresher.h"

namespace Learning2DEngine
{
	namespace System
	{
		class KeyboardMouseEventItem final : public virtual EventItem<int, int, int, int>
		{
		private:
			IKeyboardMouseRefresher* refresher;
		public:
			KeyboardMouseEventItem(IKeyboardMouseRefresher* refresher)
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