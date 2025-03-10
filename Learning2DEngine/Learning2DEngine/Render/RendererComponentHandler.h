#pragma once

#include <algorithm>

#include "../System/GameObject.h"
#include "../System/BaseComponentHandler.h"
#include "BaseRendererComponent.h"

namespace Learning2DEngine
{
    namespace Render
    {
		class RendererComponentHandler : public virtual System::BaseComponentHandler<BaseRendererComponent>
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
				: System::BaseComponentHandler<BaseRendererComponent>(), isReorderNeeded(false)
			{
			}

			void Add(BaseRendererComponent* component, bool isThreadSafe) override
			{
				BaseComponentHandler::Add(component, isThreadSafe);
				isReorderNeeded = true;
			}

			inline void NeedReorder()
			{
				isReorderNeeded = true;
			}

			void Run() override
			{
				RefreshComponents();
				ReorderComponents();

				for (auto component : components)
				{
					//A GameObject will only be destroyed at the end of the frame.
					if (component->isActive && component->gameObject->isActive)
						component->Draw();
				}
			}
		};
    }
}