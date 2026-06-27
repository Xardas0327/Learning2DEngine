#include "ColliderComponentHandler.h"

#include <algorithm>

namespace Learning2DEngine
{
	namespace Physics
	{
		ColliderComponentHandler::ColliderComponentHandler()
			: dynamicBoxColliders(), kinematicBoxColliders(), newBoxColliders(), removableBoxColliders(),
			dynamicCircleColliders(), kinematicCircleColliders(), newCircleColliders(), removableCircleColliders(),
			boxMutex(), circleMutex()
		{
		}

		void ColliderComponentHandler::RefreshBoxColliders()
		{
			if (removableBoxColliders.size() > 0)
			{
				auto newDynamicEnd = remove_if(dynamicBoxColliders.begin(), dynamicBoxColliders.end(),
					[this](BoxColliderComponent* collider)
					{
						auto it = std::find(removableBoxColliders.begin(), removableBoxColliders.end(), collider);
						return it != removableBoxColliders.end();
					});
				dynamicBoxColliders.erase(newDynamicEnd, dynamicBoxColliders.end());

				auto newKinematicEnd = remove_if(kinematicBoxColliders.begin(), kinematicBoxColliders.end(),
					[this](BoxColliderComponent* collider)
					{
						auto it = std::find(removableBoxColliders.begin(), removableBoxColliders.end(), collider);
						return it != removableBoxColliders.end();
					});
				kinematicBoxColliders.erase(newKinematicEnd, kinematicBoxColliders.end());

				removableBoxColliders.clear();
			}

			if (newBoxColliders.size() > 0)
			{
				for (auto collider : newBoxColliders)
				{
					switch (collider->type)
					{
					case ColliderType::DYNAMIC:
						dynamicBoxColliders.push_back(collider);
						break;
					case ColliderType::KINEMATIC:
						kinematicBoxColliders.push_back(collider);
						break;
					}
				}

				newBoxColliders.clear();
			}
		}

		void ColliderComponentHandler::RefreshCircleColliders()
		{
			if (removableCircleColliders.size() > 0)
			{
				auto newDynamicEnd = remove_if(dynamicCircleColliders.begin(), dynamicCircleColliders.end(),
					[this](CircleColliderComponent* collider)
					{
						auto it = std::find(removableCircleColliders.begin(), removableCircleColliders.end(), collider);
						return it != removableCircleColliders.end();
					});
				dynamicCircleColliders.erase(newDynamicEnd, dynamicCircleColliders.end());

				auto newKinematicEnd = remove_if(kinematicCircleColliders.begin(), kinematicCircleColliders.end(),
					[this](CircleColliderComponent* collider)
					{
						auto it = std::find(removableCircleColliders.begin(), removableCircleColliders.end(), collider);
						return it != removableCircleColliders.end();
					});
				kinematicCircleColliders.erase(newKinematicEnd, kinematicCircleColliders.end());

				removableCircleColliders.clear();
			}

			if (newCircleColliders.size() > 0)
			{
				for (auto collider : newCircleColliders)
				{
					switch (collider->type)
					{
					case ColliderType::DYNAMIC:
						dynamicCircleColliders.push_back(collider);
						break;
					case ColliderType::KINEMATIC:
						kinematicCircleColliders.push_back(collider);
						break;
					}
				}

				newCircleColliders.clear();
			}
		}

		void ColliderComponentHandler::CheckCollisionWithDynamicBox()
		{
			for (int i = 0; i < dynamicBoxColliders.size(); ++i)
			{
				if (!dynamicBoxColliders[i]->IsActive())
					continue;

				// dynamicBoxColliders
				if (!CheckCollisions(dynamicBoxColliders[i], dynamicBoxColliders, i + 1))
					continue;

				// dynamicCircleColliders
				if (!CheckCollisions(dynamicBoxColliders[i], dynamicCircleColliders, 0))
					continue;

				// kinematicBoxColliders
				if (!CheckCollisions(dynamicBoxColliders[i], kinematicBoxColliders, 0))
					continue;

				// passiveCircleColliders
				CheckCollisions(dynamicBoxColliders[i], kinematicCircleColliders, 0);
			}
		}

		void ColliderComponentHandler::CheckCollisionWithDynamicCircle()
		{
			// All dynamic box colliders were checked in the CheckCollisionWithActiveBox loop
			for (int i = 0; i < dynamicCircleColliders.size(); ++i)
			{
				if (!dynamicCircleColliders[i]->IsActive())
					continue;

				// dynamicCircleColliders
				if (!CheckCollisions(dynamicCircleColliders[i], dynamicCircleColliders, i + 1))
					continue;

				// kinematicBoxColliders
				if (!CheckCollisions(dynamicCircleColliders[i], kinematicBoxColliders, 0))
					continue;

				// passiveCircleColliders
				CheckCollisions(dynamicCircleColliders[i], kinematicCircleColliders, 0);
			}
		}

		void ColliderComponentHandler::Add(BoxColliderComponent* collider, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(boxMutex);
				newBoxColliders.push_back(collider);
			}
			else
			{
				newBoxColliders.push_back(collider);
			}
		}

		void ColliderComponentHandler::RemoveItem(BoxColliderComponent* collider)
		{
			//Check it, that it is a new one or not.
			auto it = std::find(newBoxColliders.begin(), newBoxColliders.end(), collider);
			if (it != newBoxColliders.end())
				newBoxColliders.erase(it);
			else
				removableBoxColliders.push_back(collider);
		}

		void ColliderComponentHandler::Remove(BoxColliderComponent* collider, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(boxMutex);
				RemoveItem(collider);
			}
			else
			{
				RemoveItem(collider);
			}
		}

		void ColliderComponentHandler::Add(CircleColliderComponent* collider, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(circleMutex);
				newCircleColliders.push_back(collider);
			}
			else
			{
				newCircleColliders.push_back(collider);
			}
		}

		void ColliderComponentHandler::RemoveItem(CircleColliderComponent* collider)
		{
			//Check it, that it is a new one or not.
			auto it = std::find(newCircleColliders.begin(), newCircleColliders.end(), collider);
			if (it != newCircleColliders.end())
				newCircleColliders.erase(it);
			else
				removableCircleColliders.push_back(collider);
		}

		void ColliderComponentHandler::Remove(CircleColliderComponent* collider, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(circleMutex);
				RemoveItem(collider);
			}
			else
			{
				RemoveItem(collider);
			}
		}

		void ColliderComponentHandler::Clear()
		{
			dynamicBoxColliders.clear();
			kinematicBoxColliders.clear();
			newBoxColliders.clear();
			removableBoxColliders.clear();

			dynamicCircleColliders.clear();
			kinematicCircleColliders.clear();
			newCircleColliders.clear();
			removableCircleColliders.clear();
		}

		void ColliderComponentHandler::Run()
		{
			RefreshBoxColliders();
			RefreshCircleColliders();

			CheckCollisionWithDynamicBox();
			CheckCollisionWithDynamicCircle();
		}
	}
}