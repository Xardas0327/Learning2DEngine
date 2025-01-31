#pragma once

#include <algorithm>

#include "GameObject.h"
#include "BaseComponentHandler.h"
#include "BaseUpdaterComponent.h"

namespace Learning2DEngine
{
	namespace System
	{
		class UpdaterComponentHandler : public virtual BaseComponentHandler<BaseUpdaterComponent>
		{
		public:
			UpdaterComponentHandler()
				: BaseComponentHandler<BaseUpdaterComponent>()
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
						component->Update();
				}
			}
		};
	}
}