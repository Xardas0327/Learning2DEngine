#pragma once

#include <algorithm>

#include "../System/GameObject.h"
#include "../System/IComponentHandler.h"
#include "BaseBoxColliderComponent.h"
#include "BaseCircleColliderComponent.h"

namespace Learning2DEngine
{
	namespace Physics
	{
		class ColliderComponentHandler : public System::IComponentHandler<BaseBoxColliderComponent>
		{
			//It doesn't matter, that this class is only for BaseBoxColliderComponent or BaseCircleColliderComponent.
			//We should use both of them.
		/*protected:
			std::vector<BaseRendererComponent*> components;
			std::vector<BaseRendererComponent*> newComponents;
			std::vector<BaseRendererComponent*> removeableComponents;

			bool isReorderNeeded;

			void ReorderComponents();
			void RefreshComponents() override;

		public:
			ColliderComponentHandler();

			void Add(BaseRendererComponent* component) override;
			void Remove(BaseRendererComponent* component) override;
			void Clear() override;
			void DoWithAllComponents() override;

			inline void NeedReorder()
			{
				isReorderNeeded = true;
			}*/
		};
	}
}