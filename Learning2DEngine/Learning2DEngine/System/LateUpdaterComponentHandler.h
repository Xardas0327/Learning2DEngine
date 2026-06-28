#pragma once

#include "GameObject.h"
#include "BaseComponentHandler.h"
#include "LateUpdaterComponent.h"

namespace Learning2DEngine
{
	namespace System
	{
		class LateUpdaterComponentHandler : public BaseComponentHandler<LateUpdaterComponent>
		{
		protected:
			virtual void RunItem(LateUpdaterComponent* component) override
			{
				//A GameObject will only be destroyed at the end of the frame.
				if (component->IsActive())
					component->LateUpdate();
			}
		public:
			LateUpdaterComponentHandler()
				: BaseComponentHandler<LateUpdaterComponent>()
			{
			}
		};
	}
}