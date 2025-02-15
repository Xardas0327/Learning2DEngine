#pragma once

#include <vector>
#include <algorithm>

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
			std::vector<BaseGameObject*> removeableGameObjects;

			GameObjectManager()
				: gameObjects(), removeableGameObjects()
			{
			}
		public:

			void Add(BaseGameObject* gameobject)
			{
				gameObjects.push_back(gameobject);
			}

			void MarkForDestroy(BaseGameObject* gameobject)
			{
				removeableGameObjects.push_back(gameobject);
			}

			void DestroyMarkedGameObjects()
			{
				if (removeableGameObjects.size() > 0)
				{
					auto newEnd = remove_if(gameObjects.begin(), gameObjects.end(),
						[this](BaseGameObject* gameObject)
						{
							auto it = std::find(removeableGameObjects.begin(), removeableGameObjects.end(), gameObject);
							if (it != removeableGameObjects.end())
							{
								delete gameObject;
								return true;
							}

							return false;
						});
					gameObjects.erase(newEnd, gameObjects.end());

					removeableGameObjects.clear();
				}
			}

			void DestroyAllGameObjects()
			{
				for(BaseGameObject* gameObject : gameObjects)
				{
					delete gameObject;
				}

				gameObjects.clear();
				removeableGameObjects.clear();
			}
		};
	}
}