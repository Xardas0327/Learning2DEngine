#include "ColliderComponentHandler.h"

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
			for (auto collider : removeableBoxColliders)
			{
				auto it = std::find(activeBoxColliders.begin(), activeBoxColliders.end(), collider);
				if (it != activeBoxColliders.end())
				{
					activeBoxColliders.erase(it);
				}
				else
				{
					it = std::find(passiveBoxColliders.begin(), passiveBoxColliders.end(), collider);
					if (it != passiveBoxColliders.end())
						passiveBoxColliders.erase(it);
				}
			}
			removeableBoxColliders.clear();

			for (auto collider : newBoxColliders)
			{
				if (collider->IsPassive())
					passiveBoxColliders.push_back(collider);
				else
					activeBoxColliders.push_back(collider);
			}
			newBoxColliders.clear();
		}

		void ColliderComponentHandler::RefreshCircleColliders()
		{
			for (auto collider : removeableCircleColliders)
			{
				auto it = std::find(activeCircleColliders.begin(), activeCircleColliders.end(), collider);
				if (it != activeCircleColliders.end())
				{
					activeCircleColliders.erase(it);
				}
				else
				{
					it = std::find(passiveCircleColliders.begin(), passiveCircleColliders.end(), collider);
					if (it != passiveCircleColliders.end())
						passiveCircleColliders.erase(it);
				}
			}
			removeableCircleColliders.clear();

			for (auto collider : newCircleColliders)
			{
				if (collider->IsPassive())
					passiveCircleColliders.push_back(collider);
				else
					activeCircleColliders.push_back(collider);
			}
			newCircleColliders.clear();
		}

		void ColliderComponentHandler::Add(BaseBoxColliderComponent* component)
		{
			newBoxColliders.push_back(component);
		}

		void ColliderComponentHandler::Remove(BaseBoxColliderComponent* component)
		{
			removeableBoxColliders.push_back(component);
		}

		void ColliderComponentHandler::Add(BaseCircleColliderComponent* component)
		{
			newCircleColliders.push_back(component);
		}

		void ColliderComponentHandler::Remove(BaseCircleColliderComponent* component)
		{
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
				// All box colliders were checked in the previous loop

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