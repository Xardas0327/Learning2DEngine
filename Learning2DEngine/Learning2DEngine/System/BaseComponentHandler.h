#pragma once

#include <vector>
#include <algorithm>

#include "IComponentHandler.h"


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

			BaseComponentHandler()
				: components(), newComponents(), removeableComponents()
			{
			}

			virtual void RefreshComponents()
			{
				for (auto component : removeableComponents)
				{
					auto it = std::find(components.begin(), components.end(), component);
					if (it != components.end())
						components.erase(it);
				}
				removeableComponents.clear();

				components.insert(components.end(), newComponents.begin(), newComponents.end());
				newComponents.clear();
			}

		public:
			virtual void Add(T* component)
			{
				newComponents.push_back(component);
			}

			virtual void Remove(T* component)
			{
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