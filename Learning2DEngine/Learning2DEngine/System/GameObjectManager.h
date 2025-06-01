#pragma once

#include <vector>
#include <algorithm>
#include <mutex>

#include "Singleton.h"
#include "GameObject.h"

namespace Learning2DEngine
{
	namespace System
	{
		class GameObjectManager final : public Singleton<GameObjectManager>
		{
			friend class Singleton<GameObjectManager>;
		private:
			std::vector<GameObject*> gameObjects;
			std::vector<GameObject*> removableGameObjects;
			std::mutex addMutex;
			std::mutex removeMutex;

			bool isThreadSafe;

			GameObjectManager()
				: gameObjects(), removableGameObjects(), addMutex(), removeMutex(), isThreadSafe(false)
			{
			}
		public:

			GameObject* CreateGameObject(bool isActive = true)
			{
				auto gameObject = new GameObject(isActive);

				if (isThreadSafe)
				{
					std::lock_guard<std::mutex> lock(addMutex);
					gameObjects.push_back(gameObject);
				}
				else
					gameObjects.push_back(gameObject);

				return gameObject;
			}

			GameObject* CreateGameObject(const Transform& transform, bool isActive = true)
			{
				auto gameObject = new GameObject(transform, isActive);

				if (isThreadSafe)
				{
					std::lock_guard<std::mutex> lock(addMutex);
					gameObjects.push_back(gameObject);
				}
				else
					gameObjects.push_back(gameObject);

				return gameObject;
			}

			/// <summary>
			/// The gameObject and its components will be destroyed.
			/// It will be inactive immediately, but it will be destroyed just end of the frame only.
			/// </summary>
			void DestroyGameObject(GameObject* gameObject)
			{
				gameObject->isActive = false;
				if (isThreadSafe)
				{
					std::lock_guard<std::mutex> lock(removeMutex);
					removableGameObjects.push_back(gameObject);
				}
				else
					removableGameObjects.push_back(gameObject);
			}

			/// <summary>
			/// The gameObject of component and its components will be destroyed.
			/// It will be inactive immediately, but it will be destroyed just end of the frame only.
			/// </summary>
			void DestroyGameObject(Component* component)
			{
				component->gameObject->isActive = false;
				if (isThreadSafe)
				{
					std::lock_guard<std::mutex> lock(removeMutex);
					removableGameObjects.push_back(component->gameObject);
				}
				else
					removableGameObjects.push_back(component->gameObject);
			}

			void DestroyMarkedGameObjects()
			{
				if (removableGameObjects.size() > 0)
				{
					auto newEnd = remove_if(gameObjects.begin(), gameObjects.end(),
						[this](GameObject* gameObject)
						{
							auto it = std::find(removableGameObjects.begin(), removableGameObjects.end(), gameObject);
							if (it != removableGameObjects.end())
							{
								gameObject->Destroy();
								delete gameObject;
								return true;
							}

							return false;
						});
					gameObjects.erase(newEnd, gameObjects.end());

					removableGameObjects.clear();
				}
			}

			void DestroyAllGameObjects()
			{
				for(GameObject* gameObject : gameObjects)
				{
					gameObject->Destroy();
					delete gameObject;
				}

				gameObjects.clear();
				removableGameObjects.clear();
			}

			inline void SetThreadSafe(bool value)
			{
				isThreadSafe = value;
			}

			inline bool GetThreadSafe()
			{
				return isThreadSafe;
			}
		};
	}
}