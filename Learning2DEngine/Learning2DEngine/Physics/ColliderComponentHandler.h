#pragma once

#include <vector>
#include <mutex>

#include "../System/GameObject.h"
#include "../System/IComponentHandler.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "CollisionHelper.h"


namespace Learning2DEngine
{
	namespace Physics
	{
		class ColliderComponentHandler : public System::IComponentHandler
		{
		protected:
			std::vector<BoxColliderComponent*> dynamicBoxColliders;
			std::vector<BoxColliderComponent*> kinematicBoxColliders;
			std::vector<BoxColliderComponent*> newBoxColliders;
			std::vector<BoxColliderComponent*> removableBoxColliders;

			std::vector<CircleColliderComponent*> dynamicCircleColliders;
			std::vector<CircleColliderComponent*> kinematicCircleColliders;
			std::vector<CircleColliderComponent*> newCircleColliders;
			std::vector<CircleColliderComponent*> removableCircleColliders;

			std::mutex boxMutex;
			std::mutex circleMutex;

			// The function returns, that the collider is still active after the OnCollision
			template<class T, class U>
			bool CheckCollisions(T* collider, std::vector<U*> colliders, size_t startIndex)
			{
				for (size_t i = startIndex; i < colliders.size(); ++i)
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
				if (second->IsActive() && (first->maskLayer & second->maskLayer))
				{
					CollisionData data = CollisionHelper::CheckCollision(*first, *second);
					if (data.isCollided)
					{
						CollisionHelper::FixPosition(*first, data.edge1, *second, data.edge2);
						first->OnCollision(Collision{ data.edge1, data.edge2, second->gameObject });
						second->OnCollision(Collision{ data.edge2, data.edge1, first->gameObject });
					}
				}

				return first->IsActive();
			}

			void RemoveItem(BoxColliderComponent* component);
			void RemoveItem(CircleColliderComponent* component);

			void RefreshBoxColliders();
			void RefreshCircleColliders();

			void CheckCollisionWithDynamicBox();
			void CheckCollisionWithDynamicCircle();

		public:
			ColliderComponentHandler();

			void Add(BoxColliderComponent* collider, bool isThreadSafe);
			void Remove(BoxColliderComponent* collider, bool isThreadSafe);

			void Add(CircleColliderComponent* collider, bool isThreadSafe);
			void Remove(CircleColliderComponent* collider, bool isThreadSafe);

			void Clear() override;
			void Run() override;
		};
	}
}