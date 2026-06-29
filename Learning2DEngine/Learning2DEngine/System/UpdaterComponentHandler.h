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
		public:
			UpdaterComponentHandler()
				: BaseComponentHandler<UpdaterComponent>()
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
									item->Update();
								}
							);
						}
						else
							item->Update();
					}
				}

				if (threadManager != nullptr)
					threadManager->WaitAll();
			}
		};
	}
}