#pragma once

#include <algorithm>

#include "../System/GameObject.h"
#include "../System/IComponentHandler.h"
#include "BaseBoxColliderComponent.h"
#include "BaseCircleColliderComponent.h"
#include "CollisionChecker.h"

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

			template<class T>
			bool IsActiveObject(T* collider, std::vector<T*> removeableColliders)
			{
				// Check if the collider is added into the removeable colliders
				auto isRemoveable = std::find(removeableColliders.begin(), removeableColliders.end(), collider);

				return isRemoveable == removeableColliders.end()
					&& collider->isActive
					&& collider->gameObject->isActive;
			}


			// The function checks, that 2 object are still active
			// and they are collided. If yes, It will call their OnCollision function.
			// IMPORTANT: The function returns, that the first object was active before the last OnCollision
			template<class T, class U>
			bool DoCollision(T* first, std::vector<T*> firstRemoveableColliders, U* second, std::vector<U*> secondRemoveableColliders)
			{
				// Check if the component is added into the removeable components in actual frame
				//Maybe the previous collision made it inactive
				if (!IsActiveObject<T>(first, firstRemoveableColliders))
					return false;

				if (IsActiveObject<U>(second, secondRemoveableColliders)
					&& (first->maskLayer & second->maskLayer))
				{
					CollisionData data = CollisionChecker::CheckCollision(*first, *second);
					if (data.isCollided)
					{
						first->OnCollision(Collision{ data.edge1, data.edge2, second->gameObject });
						second->OnCollision(Collision{ data.edge2, data.edge1, first->gameObject });
					}
				}

				return true;
			}


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