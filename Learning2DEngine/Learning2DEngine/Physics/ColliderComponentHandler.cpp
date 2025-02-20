#include "ColliderComponentHandler.h"

#include <algorithm>

namespace Learning2DEngine
{
	namespace Physics
	{
		ColliderComponentHandler::ColliderComponentHandler()
			: activeBoxColliders(), passiveBoxColliders(), newBoxColliders(), removeableBoxColliders(),
			activeCircleColliders(), passiveCircleColliders(), newCircleColliders(), removeableCircleColliders()
#if USE_THREAD
			, boxMutex(), circleMutex()
#endif
		{
		}

		void ColliderComponentHandler::RefreshBoxColliders()
		{
			if (removeableBoxColliders.size() > 0)
			{
				auto newActiveEnd = remove_if(activeBoxColliders.begin(), activeBoxColliders.end(),
					[this](BaseBoxColliderComponent* collider)
					{
						auto it = std::find(removeableBoxColliders.begin(), removeableBoxColliders.end(), collider);
						return it != removeableBoxColliders.end();
					});
				activeBoxColliders.erase(newActiveEnd, activeBoxColliders.end());

				auto newPassiveEnd = remove_if(passiveBoxColliders.begin(), passiveBoxColliders.end(),
					[this](BaseBoxColliderComponent* collider)
					{
						auto it = std::find(removeableBoxColliders.begin(), removeableBoxColliders.end(), collider);
						return it != removeableBoxColliders.end();
					});
				passiveBoxColliders.erase(newPassiveEnd, passiveBoxColliders.end());

				removeableBoxColliders.clear();
			}

			if (newBoxColliders.size() > 0)
			{
				for (auto collider : newBoxColliders)
				{
					if (collider->IsActiveCollider())
						activeBoxColliders.push_back(collider);
					else
						passiveBoxColliders.push_back(collider);
				}

				newBoxColliders.clear();
			}
		}

		void ColliderComponentHandler::RefreshCircleColliders()
		{
			if (removeableCircleColliders.size() > 0)
			{
				auto newActiveEnd = remove_if(activeCircleColliders.begin(), activeCircleColliders.end(),
					[this](BaseCircleColliderComponent* collider)
					{
						auto it = std::find(removeableCircleColliders.begin(), removeableCircleColliders.end(), collider);
						return it != removeableCircleColliders.end();
					});
				activeCircleColliders.erase(newActiveEnd, activeCircleColliders.end());

				auto newPassiveEnd = remove_if(passiveCircleColliders.begin(), passiveCircleColliders.end(),
					[this](BaseCircleColliderComponent* collider)
					{
						auto it = std::find(removeableCircleColliders.begin(), removeableCircleColliders.end(), collider);
						return it != removeableCircleColliders.end();
					});
				passiveCircleColliders.erase(newPassiveEnd, passiveCircleColliders.end());

				removeableCircleColliders.clear();
			}

			if (newCircleColliders.size() > 0)
			{
				for (auto collider : newCircleColliders)
				{
					if (collider->IsActiveCollider())
						activeCircleColliders.push_back(collider);
					else
						passiveCircleColliders.push_back(collider);
				}

				newCircleColliders.clear();
			}
		}

		void ColliderComponentHandler::Add(BaseBoxColliderComponent* component)
		{
#if USE_THREAD
			std::lock_guard<std::mutex> lock(boxMutex);
#endif
			newBoxColliders.push_back(component);
		}

		void ColliderComponentHandler::Remove(BaseBoxColliderComponent* component)
		{
#if USE_THREAD
			std::lock_guard<std::mutex> lock(boxMutex);
#endif
			//Check that it is not a new one.
			auto it = std::find(newBoxColliders.begin(), newBoxColliders.end(), component);
			if (it != newBoxColliders.end())
				newBoxColliders.erase(it);
			else
				removeableBoxColliders.push_back(component);
		}

		void ColliderComponentHandler::Add(BaseCircleColliderComponent* component)
		{
#if USE_THREAD
			std::lock_guard<std::mutex> lock(circleMutex);
#endif
			newCircleColliders.push_back(component);
		}

		void ColliderComponentHandler::Remove(BaseCircleColliderComponent* component)
		{
#if USE_THREAD
			std::lock_guard<std::mutex> lock(circleMutex);
#endif
			//Check that it is not a new one.
			auto it = std::find(newCircleColliders.begin(), newCircleColliders.end(), component);
			if (it != newCircleColliders.end())
				newCircleColliders.erase(it);
			else
				removeableCircleColliders.push_back(component);
		}

		void ColliderComponentHandler::Clear()
		{
			activeBoxColliders.clear();
			passiveBoxColliders.clear();
			newBoxColliders.clear();
			removeableBoxColliders.clear();

			activeCircleColliders.clear();
			passiveCircleColliders.clear();
			newCircleColliders.clear();
			removeableCircleColliders.clear();
		}

		void ColliderComponentHandler::CheckCollisionWithActiveBox()
		{
			for (int i = 0; i < activeBoxColliders.size(); ++i)
			{
				bool isActive = activeBoxColliders[i]->isActive && activeBoxColliders[i]->gameObject->isActive;
				if (!isActive)
					continue;

				// activeBoxColliders
				isActive = CheckCollision(activeBoxColliders[i], activeBoxColliders, i + 1);
				if (!isActive)
					continue;

				// activeCircleColliders
				isActive = CheckCollision(activeBoxColliders[i], activeCircleColliders, 0);
				if (!isActive)
					continue;

				// passiveBoxColliders
				isActive = CheckCollision(activeBoxColliders[i], passiveBoxColliders, 0);
				if (!isActive)
					continue;

				// passiveCircleColliders
				CheckCollision(activeBoxColliders[i], passiveCircleColliders, 0);
			}

		}

		void ColliderComponentHandler::CheckCollisionWithActiveCircle()
		{
			for (int i = 0; i < activeCircleColliders.size(); ++i)
			{
				// All active box colliders were checked in the previous loop
				bool isActive = activeCircleColliders[i]->isActive && activeCircleColliders[i]->gameObject->isActive;
				if (!isActive)
					continue;

				// activeCircleColliders
				isActive = CheckCollision(activeCircleColliders[i], activeCircleColliders, i + 1);
				if (!isActive)
					continue;

				// passiveBoxColliders
				isActive = CheckCollision(activeCircleColliders[i], passiveBoxColliders, 0);
				if (!isActive)
					continue;

				// passiveCircleColliders
				CheckCollision(activeCircleColliders[i], passiveCircleColliders, 0);
			}
		}

		void ColliderComponentHandler::Run()
		{
			RefreshBoxColliders();
			RefreshCircleColliders();

			CheckCollisionWithActiveBox();
			CheckCollisionWithActiveCircle();
		}

	}
}