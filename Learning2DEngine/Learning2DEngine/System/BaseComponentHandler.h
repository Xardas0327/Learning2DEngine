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
				if (removeableComponents.size() > 0)
				{
					auto newEnd = remove_if(components.begin(), components.end(),
						[this](T* component)
						{
							auto it = std::find(removeableComponents.begin(), removeableComponents.end(), component);
							return it != removeableComponents.end();
						});
					components.erase(newEnd, components.end());
				}

				components.insert(components.end(), newComponents.begin(), newComponents.end());

				removeableComponents.clear();
				newComponents.clear();
			}

		public:
			virtual void Add(T* component)
			{
				newComponents.push_back(component);
			}

			virtual void Remove(T* component)
			{
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