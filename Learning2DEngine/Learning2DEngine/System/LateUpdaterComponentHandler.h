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
		public:
			LateUpdaterComponentHandler()
				: BaseComponentHandler<LateUpdaterComponent>()
			{
			}

			virtual void Run() override
			{
				RefreshComponents();

				for (auto item : components)
				{
					if (item->IsActive())
					{
						if (threadManager != nullptr)
						{
							threadManager->Enqueue([item]
								{
									item->LateUpdate();
								}
							);
						}
						else
							item->LateUpdate();
					}
				}

				for (auto item : mainThreadOnlyComponents)
				{
					if (item->IsActive())
					{
						item->LateUpdate();
					}
				}

				if (threadManager != nullptr)
					threadManager->WaitAll();
			}
		};
	}
}