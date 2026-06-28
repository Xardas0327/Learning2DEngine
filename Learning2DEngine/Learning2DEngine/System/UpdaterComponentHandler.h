#pragma once

#include "GameObject.h"
#include "BaseComponentHandler.h"
#include "UpdaterComponent.h"

namespace Learning2DEngine
{
	namespace System
	{
		class UpdaterComponentHandler : public BaseComponentHandler<UpdaterComponent>
		{
		protected:
			virtual void RunItem(UpdaterComponent* component) override
			{
				//A GameObject will only be destroyed at the end of the frame.
				if (component->IsActive())
					component->Update();
			}
		public:
			UpdaterComponentHandler()
				: BaseComponentHandler<UpdaterComponent>()
			{
			}
		};
	}
}