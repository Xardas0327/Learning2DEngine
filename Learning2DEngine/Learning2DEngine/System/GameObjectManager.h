#pragma once

#include <vector>
#include <algorithm>

#include "Singleton.h"

namespace Learning2DEngine
{
	namespace System
	{
		class GameObject;

		class GameObjectManager : public virtual Singleton<GameObjectManager>
		{
			friend class Singleton<GameObjectManager>;
		private:
			std::vector<GameObject*> gameObjects;
			std::vector<GameObject*> removeableGameObject;

		public:
			GameObjectManager()
				: gameObjects(), removeableGameObject()
			{
			}

			void Add(GameObject* gameobject)
			{
				gameObjects.push_back(gameobject);
			}

			void MarkForDestroy(GameObject* gameobject)
			{
				removeableGameObject.push_back(gameobject);
			}

			void DestroyMarkedGameObjects()
			{
				if (removeableGameObject.size() > 0)
				{
					auto newEnd = remove_if(gameObjects.begin(), gameObjects.end(),
						[this](GameObject* gameObject)
						{
							auto it = std::find(removeableGameObject.begin(), removeableGameObject.end(), gameObject);
							if (it != removeableGameObject.end())
							{
								delete gameObject;
								return true;
							}

							return false;
						});
					gameObjects.erase(newEnd, gameObjects.end());

					removeableGameObject.clear();
				}
			}

			void DestroyAllGameObjects()
			{
				for(GameObject* gameObject : gameObjects)
				{
					delete gameObject;
				}

				gameObjects.clear();
				removeableGameObject.clear();
			}
		};
	}
}