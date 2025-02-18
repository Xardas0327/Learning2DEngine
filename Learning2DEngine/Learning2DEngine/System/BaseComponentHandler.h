#pragma once

#include <vector>
#include <algorithm>

#include "EngineMacro.h"
#include "IComponentHandler.h"

#if USE_THREAD
#include <mutex>
#endif

namespace Learning2DEngine
{
	namespace System
	{
		template<class T>
		class BaseComponentHandler : public IComponentHandler
		{
		protected:
			std::vector<T*> components;
			std::vector<T*> newComponents;
			std::vector<T*> removeableComponents;
#if USE_THREAD
			std::mutex mutex;
#endif

			BaseComponentHandler()
				: components(), newComponents(), removeableComponents()
#if USE_THREAD
				, mutex()
#endif
			{
			}

			virtual void RefreshComponents()
			{
				if (removeableComponents.size() > 0)
				{
					auto newEnd = remove_if(components.begin(), components.end(),
						[this](T* component)
						{
							auto it = std::find(removeableComponents.begin(), removeableComponents.end(), component);
							return it != removeableComponents.end();
						});
					components.erase(newEnd, components.end());

					removeableComponents.clear();
				}

				if (newComponents.size() > 0)
				{
					components.insert(components.end(), newComponents.begin(), newComponents.end());
					newComponents.clear();
				}
			}

		public:
			virtual void Add(T* component)
			{
#if USE_THREAD
				std::lock_guard<std::mutex> lock(mutex);
				int a = 1;
#endif
				newComponents.push_back(component);
			}

			virtual void Remove(T* component)
			{
#if USE_THREAD
				std::lock_guard<std::mutex> lock(mutex);
#endif
				//Check that it is not a new one.
				auto it = std::find(newComponents.begin(), newComponents.end(), component);
				if (it != newComponents.end())
					newComponents.erase(it);
				else
					removeableComponents.push_back(component);
			}

			virtual void Clear() override
			{
				components.clear();
				newComponents.clear();
				removeableComponents.clear();
			}
		};
	}
}