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
		class ColliderComponentHandler : public System::IComponentHandler
		{
		protected:
			std::vector<BaseBoxColliderComponent*> activeBoxColliders;
			std::vector<BaseBoxColliderComponent*> passiveBoxColliders;
			std::vector<BaseBoxColliderComponent*> newBoxColliders;
			std::vector<BaseBoxColliderComponent*> removeableBoxColliders;

			std::vector<BaseCircleColliderComponent*> activeCircleColliders;
			std::vector<BaseCircleColliderComponent*> passiveCircleColliders;
			std::vector<BaseCircleColliderComponent*> newCircleColliders;
			std::vector<BaseCircleColliderComponent*> removeableCircleColliders;


			void RefreshBoxColliders();
			void RefreshCircleColliders();

		public:
			ColliderComponentHandler();

			void Add(BaseBoxColliderComponent* collider);
			void Remove(BaseBoxColliderComponent* collider);

			void Add(BaseCircleColliderComponent* collider);
			void Remove(BaseCircleColliderComponent* collider);

			void Clear() override;
			void DoWithAllComponents() override;
		};
	}
}