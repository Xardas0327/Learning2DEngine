#include "ColliderComponentHandler.h"
#include "CollisionChecker.h"

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

		void ColliderComponentHandler::DoWithAllComponents()
		{
			RefreshBoxColliders();
			RefreshCircleColliders();

			//TODO: Implement collision detection
		}

	}
}