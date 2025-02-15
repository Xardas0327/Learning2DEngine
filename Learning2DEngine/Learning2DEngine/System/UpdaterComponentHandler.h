#pragma once

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
					//A GameObject will only be destroyed at the end of the frame.
					if (component->isActive && component->gameObject->isActive)
						component->Update();
				}
			}
		};
	}
}