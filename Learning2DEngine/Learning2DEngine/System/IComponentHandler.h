#pragma once

#include <vector>

namespace Learning2DEngine
{
	namespace System
	{
		template<class T>
		class IComponentHandler
		{
		protected:
			virtual void RefreshComponents() = 0;

		public:
			virtual void Add(T* component) = 0;

			virtual void Remove(T* component) = 0;

			virtual void Clear() = 0;

			virtual void DoWithAllComponents() = 0;
		};
	}
}
