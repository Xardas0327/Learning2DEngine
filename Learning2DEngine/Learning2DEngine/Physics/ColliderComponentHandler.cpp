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

		void ColliderComponentHandler::RunActiveColliderPart(size_t startIndex, size_t endIndex)
		{
			size_t end;
			if (startIndex < activeBoxColliders.size())
			{
				end = endIndex > activeBoxColliders.size() ? activeBoxColliders.size() : endIndex;
				for (size_t i = startIndex; i < end; ++i)
				{
					if (!activeBoxColliders[i]->isActive || !activeBoxColliders[i]->gameObject->isActive)
						continue;

					// activeBoxColliders
					if (!CheckCollisions(activeBoxColliders[i], activeBoxColliders, i + 1))
						continue;

					// activeCircleColliders
					CheckCollisions(activeBoxColliders[i], activeCircleColliders, 0);
				}

				// decrease the indexes for another loop
				startIndex = 0;
				endIndex -= end;
			}
			else
			{
				// decrease the indexes for another loop
				startIndex -= activeBoxColliders.size();
				endIndex -= activeBoxColliders.size();
			}

			end = endIndex > activeCircleColliders.size() ? activeCircleColliders.size() : endIndex;
			for (size_t i = startIndex; i < endIndex; ++i)
			{
				// All active box colliders were checked in the previous loop
				if (!activeCircleColliders[i]->isActive || !activeCircleColliders[i]->gameObject->isActive)
					continue;

				// activeCircleColliders
				CheckCollisions(activeCircleColliders[i], activeCircleColliders, i + 1);
			}
		}

		void ColliderComponentHandler::RunPassiveColliderPart(size_t startIndex, size_t endIndex)
		{
			size_t end;
			if (startIndex < passiveBoxColliders.size())
			{
				end = endIndex > passiveBoxColliders.size() ? passiveBoxColliders.size() : endIndex;
				for (size_t i = startIndex; i < end; ++i)
				{
					if (!passiveBoxColliders[i]->isActive || !passiveBoxColliders[i]->gameObject->isActive)
						continue;

					// activeBoxColliders
					if (!CheckCollisions(passiveBoxColliders[i], activeBoxColliders, 0))
						continue;

					// activeCircleColliders
					CheckCollisions(passiveBoxColliders[i], activeCircleColliders, 0);
				}

				// decrease the indexes for another loop
				startIndex = 0;
				endIndex -= end;
			}
			else
			{
				// decrease the indexes for another loop
				startIndex -= passiveBoxColliders.size();
				endIndex -= passiveBoxColliders.size();
			}

			end = endIndex > passiveCircleColliders.size() ? passiveCircleColliders.size() : endIndex;
			for (size_t i = startIndex; i < endIndex; ++i)
			{
				if (!passiveCircleColliders[i]->isActive || !passiveCircleColliders[i]->gameObject->isActive)
					continue;

				// activeBoxColliders
				if (!CheckCollisions(passiveCircleColliders[i], activeBoxColliders, 0))
					continue;

				// activeCircleColliders
				CheckCollisions(passiveCircleColliders[i], activeCircleColliders, 0);
			}
		}

		void ColliderComponentHandler::RunOnThreads()
		{
			//Missing yet
		}

		void ColliderComponentHandler::Run()
		{
			RefreshBoxColliders();
			RefreshCircleColliders();

			int allActiveColliders = activeBoxColliders.size() + activeCircleColliders.size();
			int allPassiveColliders = passiveBoxColliders.size() + passiveCircleColliders.size();

			int allColliderChecking = allActiveColliders * (allActiveColliders - 1) / 2 + allActiveColliders * allPassiveColliders;

			if (maxCheckingPerThread == 0 || allColliderChecking < maxCheckingPerThread)
			{
				RunActiveColliderPart(0, allActiveColliders);
				RunPassiveColliderPart(0, allPassiveColliders);
			}
			else
				RunOnThreads();
		}

	}
}