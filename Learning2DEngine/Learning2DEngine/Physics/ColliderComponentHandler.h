#pragma once

#include <algorithm>
#include <mutex>

#include "../System/EngineMacro.h"
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

			std::mutex boxMutex;
			std::mutex circleMutex;

			// The function returns, that the collider is still active after the OnCollision
			template<class T, class U>
			bool CheckCollision(T* collider, std::vector<U*> colliders, int startIndex)
			{
				for (int i = startIndex; i < colliders.size(); ++i)
				{
					if (!CheckCollision(collider, colliders[i]))
						return false;
				}

				return true;
			}

			// The function checks only the second object is still active
			// and they are collided. If yes, It will call their OnCollision function.
			// IMPORTANT: The function returns, that the first object is still active after the OnCollision
			template<class T, class U>
			bool CheckCollision(T* first, U* second)
			{
				if (second->isActive && second->gameObject->isActive
					&& (first->maskLayer & second->maskLayer))
				{
					CollisionData data = CollisionChecker::CheckCollision(*first, *second);
					if (data.isCollided)
					{
						first->OnCollision(Collision{ data.edge1, data.edge2, second->gameObject });
						second->OnCollision(Collision{ data.edge2, data.edge1, first->gameObject });
					}
				}

				return first->isActive && first->gameObject->isActive;
			}

			void RemoveItem(BaseBoxColliderComponent* component);
			void RemoveItem(BaseCircleColliderComponent* component);

			void RefreshBoxColliders();
			void RefreshCircleColliders();
			void CheckCollisionWithActiveBox();
			void CheckCollisionWithActiveCircle();

		public:
			ColliderComponentHandler();

			void Add(BaseBoxColliderComponent* collider, bool isThreadSafe);
			void Remove(BaseBoxColliderComponent* collider, bool isThreadSafe);

			void Add(BaseCircleColliderComponent* collider, bool isThreadSafe);
			void Remove(BaseCircleColliderComponent* collider, bool isThreadSafe);

			void Clear() override;
			void Run() override;
		};
	}
}