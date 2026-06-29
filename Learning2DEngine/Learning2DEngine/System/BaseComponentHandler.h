#pragma once

#include <vector>
#include <algorithm>
#include <mutex>

#include "IComponentHandler.h"
#include "ThreadManager.h"

namespace Learning2DEngine
{
	namespace System
	{
		template<class T>
		class BaseComponentHandler : public IComponentHandler
		{
		protected:
			std::vector<T*> components;
			std::vector<T*> mainThreadOnlyComponents;
			std::vector<T*> newComponents;
			std::vector<T*> removableComponents;
			std::mutex mutex;
			ThreadManager* threadManager;

			BaseComponentHandler()
				: components(), mainThreadOnlyComponents(), newComponents(), removableComponents(), mutex(), threadManager(nullptr)
			{
			}

			void RefreshComponents()
			{
				if (removableComponents.size() > 0)
				{
					auto newEnd = remove_if(components.begin(), components.end(),
						[this](T* component)
						{
							auto it = std::find(removableComponents.begin(), removableComponents.end(), component);
							return it != removableComponents.end();
						});
					components.erase(newEnd, components.end());

					newEnd = remove_if(mainThreadOnlyComponents.begin(), mainThreadOnlyComponents.end(),
						[this](T* component)
						{
							auto it = std::find(removableComponents.begin(), removableComponents.end(), component);
							return it != removableComponents.end();
						});
					mainThreadOnlyComponents.erase(newEnd, mainThreadOnlyComponents.end());

					removableComponents.clear();
				}

				for (auto newComponent : newComponents)
				{
					if (newComponent->isUseMainThreadOnly)
						mainThreadOnlyComponents.push_back(newComponent);
					else
						components.push_back(newComponent);
				}
				newComponents.clear();
			}

			void RemoveItem(T* component)
			{
				//Check it, that it is a new one or not.
				auto it = std::find(newComponents.begin(), newComponents.end(), component);
				if (it != newComponents.end())
					newComponents.erase(it);
				else
					removableComponents.push_back(component);
			}
		public:
			void Add(T* component, bool isThreadSafe)
			{
				if (isThreadSafe)
				{
					std::lock_guard<std::mutex> lock(mutex);
					newComponents.push_back(component);
				}
				else
				{
					newComponents.push_back(component);
				}
			}

			void Remove(T* component, bool isThreadSafe)
			{
				if (isThreadSafe)
				{
					std::lock_guard<std::mutex> lock(mutex);
					RemoveItem(component);
				}
				else
				{
					RemoveItem(component);
				}
			}

			inline void SetThreadManager(ThreadManager* threadManager)
			{
				this->threadManager = threadManager;
			}

			inline void ClearThreadManager()
			{
				threadManager = nullptr;
			}

			inline bool IsUseThreads() const
			{
				return threadManager != nullptr;
			}

			virtual void Clear() override
			{
				components.clear();
				mainThreadOnlyComponents.clear();
				newComponents.clear();
				removableComponents.clear();
			}
		};
	}
}