#pragma once

#include <algorithm>
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
		//If it uses multiple threads, the order of OnCollision will not be deterministic.
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

			std::vector<std::thread> threads;
			//If it is 0, the class will not use threads
			unsigned int maxColliderPerThread;

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
				if (second->isActive && second->gameObject->isActive
					&& (first->maskLayer & second->maskLayer))
				{
					CollisionData data = CollisionHelper::CheckCollision(*first, *second);
					if (data.isCollided)
					{
						CollisionHelper::FixPosition(*first, data.edge1, *second, data.edge2);
						first->OnCollision(Collision{ data.edge1, data.edge2, second->gameObject });
						second->OnCollision(Collision{ data.edge2, data.edge1, first->gameObject });
					}
				}

				return first->isActive && first->gameObject->isActive;
			}

			void RemoveItem(BoxColliderComponent* component);
			void RemoveItem(CircleColliderComponent* component);

			void RefreshBoxColliders();
			void RefreshCircleColliders();

			/// <param name="startIndex">Inclusive</param>
			/// <param name="endIndex">Exclusive</param>
			void RunDynamicColliderPart(size_t startIndex, size_t endIndex);

			/// <param name="startIndex">Inclusive</param>
			/// <param name="endIndex">Exclusive</param>
			void RunKinematicColliderPart(size_t startIndex, size_t endIndex);
			void RunOnThreads();

			inline size_t GetDynamicColliderNumber()
			{
				return dynamicBoxColliders.size() + dynamicCircleColliders.size();
			}

			inline size_t GetKinematicColliderNumber()
			{
				return kinematicBoxColliders.size() + kinematicCircleColliders.size();
			}

		public:
			ColliderComponentHandler();

			void Add(BoxColliderComponent* collider, bool isThreadSafe);
			void Remove(BoxColliderComponent* collider, bool isThreadSafe);

			void Add(CircleColliderComponent* collider, bool isThreadSafe);
			void Remove(CircleColliderComponent* collider, bool isThreadSafe);

			void Clear() override;
			void Run() override;

			//If it is 0, the class will not use threads
			//If it uses multiple threads, the order of OnCollision will not be deterministic.
			inline void SetMaxColliderPerThread(unsigned int value)
			{
				maxColliderPerThread = value;
			}

			//If it is 0, the class will not use threads
			inline unsigned int GetMaxColliderPerThread()
			{
				return maxColliderPerThread;
			}
		};
	}
}