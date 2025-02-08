#pragma once

namespace Learning2DEngine
{
	namespace System
	{
		class ICursorRefresher
		{
		public:
			virtual ~ICursorRefresher() {}

			virtual void RefreshMouseButton(int button, int action, int mods) = 0;

			virtual void RefreshPosition(double xpos, double ypos) = 0;

			virtual void RefreshIsInWindows(bool entered) = 0;

			virtual void RefreshScroll(double xoffset, double yoffset) = 0;
		};
	}
}