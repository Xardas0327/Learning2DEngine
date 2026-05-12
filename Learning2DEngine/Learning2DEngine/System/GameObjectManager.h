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

			GameObjectManager();
		public:

			GameObject* CreateGameObject(
				bool isActive = true,
				glm::vec2 position = glm::vec2(0.0f, 0.0f),
				glm::vec2 scale = glm::vec2(1.0f, 1.0f),
				float rotation = 0.0f);
			GameObject* CreateGameObject(const Transform& transform, bool isActive = true);

			/// <summary>
			/// The gameObject and its components will be destroyed.
			/// The gameObject will be active and run in this frame, and it will be destroyed just end of the frame only.
			/// </summary>
			void DestroyGameObject(GameObject* gameObject);

			/// <summary>
			/// The gameObject of component and its components will be destroyed.
			/// The gameObject will be active and run in this frame, and it will be destroyed just end of the frame only.
			/// </summary>
			void DestroyGameObject(Component* component);

			void DestroyMarkedGameObjects();

			void DestroyAllGameObjects();

			inline void Reserve(size_t value)
			{
				gameObjects.reserve(value);
			}

			// It can the reserve() with current size + value
			inline void AddReserve(size_t value)
			{
				gameObjects.reserve(gameObjects.size() + value);
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