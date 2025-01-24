#pragma once

#include <algorithm>

#include "../System/GameObject.h"
#include "../System/IComponentHandler.h"
#include "BaseRendererComponent.h"

namespace Learning2DEngine
{
    namespace Render
    {
		class RendererComponentHandler : public System::IComponentHandler<BaseRendererComponent>
		{
		protected:
			std::vector<BaseRendererComponent*> components;
			std::vector<BaseRendererComponent*> newComponents;
			std::vector<BaseRendererComponent*> removeableComponents;

			bool isReorderNeeded;

			void ReorderComponents()
			{
				if (!isReorderNeeded)
					return;

				isReorderNeeded = false;

				std::sort(components.begin(), components.end(), [](BaseRendererComponent* a, BaseRendererComponent* b)
					{
						return a->GetLayer() < b->GetLayer();
					});
			}

			void RefreshComponents() override
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
			RendererComponentHandler()
				: components(), newComponents(), removeableComponents(), isReorderNeeded(false)
			{
			}

			void Add(BaseRendererComponent* component) override
			{
				newComponents.push_back(component);
				isReorderNeeded = true;
			}

			void Remove(BaseRendererComponent* component) override
			{
				removeableComponents.push_back(component);
			}

			inline void NeedReorder()
			{
				isReorderNeeded = true;
			}

			void Clear() override
			{
				components.clear();
				newComponents.clear();
				removeableComponents.clear();
			}
			
			void DoWithAllComponents() override
			{
				RefreshComponents();
				ReorderComponents();

				for (auto component : components)
				{
					// Check if the component is added into the removeableComponents in actual frame
					auto isRemoveable = std::find(removeableComponents.begin(), removeableComponents.end(), component);

					if (isRemoveable == removeableComponents.end()
						&& component->isActive && component->gameObject->isActive)
						component->Draw();
				}
			}
		};
    }
}