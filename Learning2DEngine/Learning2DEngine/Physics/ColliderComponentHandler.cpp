#include "ColliderComponentHandler.h"

#include <algorithm>

namespace Learning2DEngine
{
	namespace Physics
	{
		ColliderComponentHandler::ColliderComponentHandler()
			: activeBoxColliders(), passiveBoxColliders(), newBoxColliders(), removeableBoxColliders(),
			activeCircleColliders(), passiveCircleColliders(), newCircleColliders(), removeableCircleColliders()
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

			for (auto collider : newBoxColliders)
			{
				if (collider->IsActiveCollider())
					activeBoxColliders.push_back(collider);
				else
					passiveBoxColliders.push_back(collider);
			}

			newBoxColliders.clear();
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

			for (auto collider : newCircleColliders)
			{
				if (collider->IsActiveCollider())
					activeCircleColliders.push_back(collider);
				else
					passiveCircleColliders.push_back(collider);
			}

			newCircleColliders.clear();
		}

		void ColliderComponentHandler::Add(BaseBoxColliderComponent* component)
		{
			newBoxColliders.push_back(component);
		}

		void ColliderComponentHandler::Remove(BaseBoxColliderComponent* component)
		{
			//Check that it is not a new one.
			auto it = std::find(newBoxColliders.begin(), newBoxColliders.end(), component);
			if (it != newBoxColliders.end())
				newBoxColliders.erase(it);
			else
				removeableBoxColliders.push_back(component);
		}

		void ColliderComponentHandler::Add(BaseCircleColliderComponent* component)
		{
			newCircleColliders.push_back(component);
		}

		void ColliderComponentHandler::Remove(BaseCircleColliderComponent* component)
		{
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
				bool isActive = true;
				// activeBoxColliders
				for (int j = i + 1; j < activeBoxColliders.size(); ++j)
				{
					isActive = DoCollision(
						activeBoxColliders[i],
						removeableBoxColliders,
						activeBoxColliders[j],
						removeableBoxColliders);

					if (!isActive)
						break;
				}

				if (!isActive)
					continue;

				// activeCircleColliders
				for (int j = 0; j < activeCircleColliders.size(); ++j)
				{

					isActive = DoCollision(
						activeBoxColliders[i],
						removeableBoxColliders,
						activeCircleColliders[j],
						removeableCircleColliders);

					if (!isActive)
						break;
				}

				if (!isActive)
					continue;

				// passiveBoxColliders
				for (int j = 0; j < passiveBoxColliders.size(); ++j)
				{

					isActive = DoCollision(
						activeBoxColliders[i],
						removeableBoxColliders,
						passiveBoxColliders[j],
						removeableBoxColliders);

					if (!isActive)
						break;
				}

				if (!isActive)
					continue;

				// passiveCircleColliders
				for (int j = 0; j < passiveCircleColliders.size(); ++j)
				{

					isActive = DoCollision(
						activeBoxColliders[i],
						removeableBoxColliders,
						passiveCircleColliders[j],
						removeableCircleColliders);

					if (!isActive)
						break;
				}
			}

		}

		void ColliderComponentHandler::CheckCollisionWithActiveCircle()
		{
			for (int i = 0; i < activeCircleColliders.size(); ++i)
			{
				// All active box colliders were checked in the previous loop

				bool isActive = true;
				// activeCircleColliders
				for (int j = i + 1; j < activeCircleColliders.size(); ++j)
				{
					isActive = DoCollision(
						activeCircleColliders[i],
						removeableCircleColliders,
						activeCircleColliders[j],
						removeableCircleColliders);

					if (!isActive)
						break;
				}

				if (!isActive)
					continue;

				// passiveBoxColliders
				for (int j = 0; j < passiveBoxColliders.size(); ++j)
				{

					isActive = DoCollision(
						activeCircleColliders[i],
						removeableCircleColliders,
						passiveBoxColliders[j],
						removeableBoxColliders);

					if (!isActive)
						break;
				}

				if (!isActive)
					continue;

				// passiveCircleColliders
				for (int j = 0; j < passiveCircleColliders.size(); ++j)
				{

					isActive = DoCollision(
						activeCircleColliders[i],
						removeableCircleColliders,
						passiveCircleColliders[j],
						removeableCircleColliders);

					if (!isActive)
						break;
				}
			}
		}

		void ColliderComponentHandler::DoWithAllComponents()
		{
			RefreshBoxColliders();
			RefreshCircleColliders();

			CheckCollisionWithActiveBox();
			CheckCollisionWithActiveCircle();
		}

	}
}