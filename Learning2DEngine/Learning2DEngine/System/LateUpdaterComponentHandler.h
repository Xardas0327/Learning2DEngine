#pragma once

#include <algorithm>

#include "GameObject.h"
#include "BaseComponentHandler.h"
#include "BaseLateUpdaterComponent.h"

namespace Learning2DEngine
{
	namespace System
	{
		class LateUpdaterComponentHandler : public virtual BaseComponentHandler<BaseLateUpdaterComponent>
		{
		public:
			LateUpdaterComponentHandler()
				: BaseComponentHandler<BaseLateUpdaterComponent>()
			{
			}

			void DoWithAllComponents() override
			{
				RefreshComponents();

				for (auto component : components)
				{
					// Check if the component is added into the removeableComponents in actual frame
					auto isRemoveable = std::find(removeableComponents.begin(), removeableComponents.end(), component);

					if (isRemoveable == removeableComponents.end()
						&& component->isActive && component->gameObject->isActive)
						component->LateUpdate();
				}
			}
		};
	}
}