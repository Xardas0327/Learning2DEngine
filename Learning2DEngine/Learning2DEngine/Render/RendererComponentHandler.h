#pragma once

#include <algorithm>

#include "../System/GameObject.h"
#include "../System/BaseComponentHandler.h"
#include "BaseRendererComponent.h"

namespace Learning2DEngine
{
    namespace Render
    {
		class RendererComponentHandler : public System::BaseComponentHandler<BaseRendererComponent>
		{
		protected:
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

		public:
			RendererComponentHandler()
				: BaseComponentHandler(), isReorderNeeded(false)
			{
			}

			void Add(BaseRendererComponent* component) override
			{
				BaseComponentHandler::Add(component);
				isReorderNeeded = true;
			}

			inline void NeedReorder()
			{
				isReorderNeeded = true;
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