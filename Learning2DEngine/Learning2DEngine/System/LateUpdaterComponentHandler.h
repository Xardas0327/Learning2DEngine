#pragma once

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
					//A GameObject will only be destroyed at the end of the frame.
					if (component->isActive && component->gameObject->isActive)
						component->LateUpdate();
				}
			}
		};
	}
}