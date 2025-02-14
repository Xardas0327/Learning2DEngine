#pragma once

#include <vector>
#include <algorithm>

#include "Singleton.h"
#include "BaseGameObject.h"

namespace Learning2DEngine
{
	namespace System
	{
		class GameObjectManager : public virtual Singleton<GameObjectManager>
		{
			friend class Singleton<GameObjectManager>;
		private:
			std::vector<BaseGameObject*> gameObjects;
			std::vector<BaseGameObject*> removeableGameObject;

		public:
			GameObjectManager()
				: gameObjects(), removeableGameObject()
			{
			}

			void Add(BaseGameObject* gameobject)
			{
				gameObjects.push_back(gameobject);
			}

			void MarkForDestroy(BaseGameObject* gameobject)
			{
				removeableGameObject.push_back(gameobject);
			}

			void DestroyMarkedGameObjects()
			{
				if (removeableGameObject.size() > 0)
				{
					auto newEnd = remove_if(gameObjects.begin(), gameObjects.end(),
						[this](BaseGameObject* gameObject)
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
				for(BaseGameObject* gameObject : gameObjects)
				{
					delete gameObject;
				}

				gameObjects.clear();
				removeableGameObject.clear();
			}
		};
	}
}