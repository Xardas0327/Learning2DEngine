#pragma once

#include <vector>

namespace Learning2DEngine
{
	namespace System
	{
		class IComponentHandler
		{
		protected:
			virtual void RefreshComponents() = 0;

		public:
			virtual void Clear() = 0;

			virtual void DoWithAllComponents() = 0;
		};
	}
}
