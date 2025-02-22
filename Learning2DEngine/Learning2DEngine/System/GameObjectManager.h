#pragma once

#include <vector>
#include <algorithm>
#include <mutex>

#include "Singleton.h"
#include "BaseGameObject.h"

namespace Learning2DEngine
{
	namespace System
	{
		class GameObjectManager final : public virtual Singleton<GameObjectManager>
		{
			friend class Singleton<GameObjectManager>;
		private:
			std::vector<BaseGameObject*> gameObjects;
			std::vector<BaseGameObject*> removableGameObjects;
			std::mutex addMutex;
			std::mutex removeMutex;

			bool isThreadSafe;

			GameObjectManager()
				: gameObjects(), removableGameObjects(), addMutex(), removeMutex(), isThreadSafe(false)
			{
			}
		public:

			void Add(BaseGameObject* gameobject)
			{
				if (isThreadSafe)
				{
					std::lock_guard<std::mutex> lock(addMutex);
					gameObjects.push_back(gameobject);
				}
				else
					gameObjects.push_back(gameobject);
			}

			void MarkForDestroy(BaseGameObject* gameobject)
			{
				if (isThreadSafe)
				{
					std::lock_guard<std::mutex> lock(removeMutex);
					removableGameObjects.push_back(gameobject);
				}
				else
					removableGameObjects.push_back(gameobject);
			}

			void DestroyMarkedGameObjects()
			{
				if (removableGameObjects.size() > 0)
				{
					auto newEnd = remove_if(gameObjects.begin(), gameObjects.end(),
						[this](BaseGameObject* gameObject)
						{
							auto it = std::find(removableGameObjects.begin(), removableGameObjects.end(), gameObject);
							if (it != removableGameObjects.end())
							{
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
				for(BaseGameObject* gameObject : gameObjects)
				{
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