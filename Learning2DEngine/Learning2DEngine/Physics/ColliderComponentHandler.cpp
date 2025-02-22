#include "ColliderComponentHandler.h"

#include <algorithm>

namespace Learning2DEngine
{
	namespace Physics
	{
		ColliderComponentHandler::ColliderComponentHandler()
			: activeBoxColliders(), passiveBoxColliders(), newBoxColliders(), removableBoxColliders(),
			activeCircleColliders(), passiveCircleColliders(), newCircleColliders(), removableCircleColliders(),
			boxMutex(), circleMutex(), threads(), maxCheckingPerThread()
		{
		}

		void ColliderComponentHandler::RefreshBoxColliders()
		{
			if (removableBoxColliders.size() > 0)
			{
				auto newActiveEnd = remove_if(activeBoxColliders.begin(), activeBoxColliders.end(),
					[this](BaseBoxColliderComponent* collider)
					{
						auto it = std::find(removableBoxColliders.begin(), removableBoxColliders.end(), collider);
						return it != removableBoxColliders.end();
					});
				activeBoxColliders.erase(newActiveEnd, activeBoxColliders.end());

				auto newPassiveEnd = remove_if(passiveBoxColliders.begin(), passiveBoxColliders.end(),
					[this](BaseBoxColliderComponent* collider)
					{
						auto it = std::find(removableBoxColliders.begin(), removableBoxColliders.end(), collider);
						return it != removableBoxColliders.end();
					});
				passiveBoxColliders.erase(newPassiveEnd, passiveBoxColliders.end());

				removableBoxColliders.clear();
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
			if (removableCircleColliders.size() > 0)
			{
				auto newActiveEnd = remove_if(activeCircleColliders.begin(), activeCircleColliders.end(),
					[this](BaseCircleColliderComponent* collider)
					{
						auto it = std::find(removableCircleColliders.begin(), removableCircleColliders.end(), collider);
						return it != removableCircleColliders.end();
					});
				activeCircleColliders.erase(newActiveEnd, activeCircleColliders.end());

				auto newPassiveEnd = remove_if(passiveCircleColliders.begin(), passiveCircleColliders.end(),
					[this](BaseCircleColliderComponent* collider)
					{
						auto it = std::find(removableCircleColliders.begin(), removableCircleColliders.end(), collider);
						return it != removableCircleColliders.end();
					});
				passiveCircleColliders.erase(newPassiveEnd, passiveCircleColliders.end());

				removableCircleColliders.clear();
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

		void ColliderComponentHandler::Add(BaseBoxColliderComponent* component, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(boxMutex);
				newBoxColliders.push_back(component);
			}
			else
			{
				newBoxColliders.push_back(component);
			}
		}

		void ColliderComponentHandler::RemoveItem(BaseBoxColliderComponent* component)
		{
			//Check it, that it is a new one or not.
			auto it = std::find(newBoxColliders.begin(), newBoxColliders.end(), component);
			if (it != newBoxColliders.end())
				newBoxColliders.erase(it);
			else
				removableBoxColliders.push_back(component);
		}

		void ColliderComponentHandler::Remove(BaseBoxColliderComponent* component, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(boxMutex);
				RemoveItem(component);
			}
			else
			{
				RemoveItem(component);
			}
		}

		void ColliderComponentHandler::Add(BaseCircleColliderComponent* component, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(circleMutex);
				newCircleColliders.push_back(component);
			}
			else
			{
				newCircleColliders.push_back(component);
			}
		}

		void ColliderComponentHandler::RemoveItem(BaseCircleColliderComponent* component)
		{
			//Check it, that it is a new one or not.
			auto it = std::find(newCircleColliders.begin(), newCircleColliders.end(), component);
			if (it != newCircleColliders.end())
				newCircleColliders.erase(it);
			else
				removableCircleColliders.push_back(component);
		}

		void ColliderComponentHandler::Remove(BaseCircleColliderComponent* component, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(circleMutex);
				RemoveItem(component);
			}
			else
			{
				RemoveItem(component);
			}
		}

		void ColliderComponentHandler::Clear()
		{
			activeBoxColliders.clear();
			passiveBoxColliders.clear();
			newBoxColliders.clear();
			removableBoxColliders.clear();

			activeCircleColliders.clear();
			passiveCircleColliders.clear();
			newCircleColliders.clear();
			removableCircleColliders.clear();
		}

		void ColliderComponentHandler::CheckCollisionWithActiveBox()
		{
			for (int i = 0; i < activeBoxColliders.size(); ++i)
			{
				bool isActive = activeBoxColliders[i]->isActive && activeBoxColliders[i]->gameObject->isActive;
				if (!isActive)
					continue;

				// activeBoxColliders
				isActive = CheckCollisions(activeBoxColliders[i], activeBoxColliders, i + 1);
				if (!isActive)
					continue;

				// activeCircleColliders
				isActive = CheckCollisions(activeBoxColliders[i], activeCircleColliders, 0);
				if (!isActive)
					continue;

				// passiveBoxColliders
				isActive = CheckCollisions(activeBoxColliders[i], passiveBoxColliders, 0);
				if (!isActive)
					continue;

				// passiveCircleColliders
				CheckCollisions(activeBoxColliders[i], passiveCircleColliders, 0);
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
				isActive = CheckCollisions(activeCircleColliders[i], activeCircleColliders, i + 1);
				if (!isActive)
					continue;

				// passiveBoxColliders
				isActive = CheckCollisions(activeCircleColliders[i], passiveBoxColliders, 0);
				if (!isActive)
					continue;

				// passiveCircleColliders
				CheckCollisions(activeCircleColliders[i], passiveCircleColliders, 0);
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