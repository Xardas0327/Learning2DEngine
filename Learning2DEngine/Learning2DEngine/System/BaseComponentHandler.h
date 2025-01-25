#pragma once

#include <vector>

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

			virtual void RefreshComponents() override
			{
				for (auto component : removeableComponents)
				{
					components.erase(std::remove(components.begin(), components.end(), component), components.end());
				}
				removeableComponents.clear();

				for (auto component : newComponents)
				{
					components.push_back(component);
				}
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