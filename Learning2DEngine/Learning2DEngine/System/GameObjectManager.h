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
			static constexpr const glm::vec2 DefaultPosition = glm::vec2(0.0f, 0.0f);
			static constexpr const glm::vec2 DefaultScale = glm::vec2(1.0f, 1.0f);
			static constexpr const float DefaultRotation = 0.0f;

			std::vector<GameObject*> gameObjects;
			std::vector<GameObject*> removableGameObjects;
			std::mutex addMutex;
			std::mutex removeMutex;

			bool isThreadSafe;

			GameObjectManager();

			void MarkGameObjectForRemoval(GameObject* gameObject);
		public:

			GameObject* CreateGameObject(
				glm::vec2 position = DefaultPosition,
				glm::vec2 scale = DefaultScale,
				float rotation = DefaultRotation,
				bool isActive = true);

			GameObject* CreateGameObject(glm::vec2 position, bool isActive);

			GameObject* CreateGameObject(bool isActive);

			/// <summary>
			/// The gameObject, their children and their components will be destroyed.
			/// The gameObject(s) will be active and run in this frame, and it will be destroyed just end of the frame only.
			/// </summary>
			void DestroyGameObject(GameObject* gameObject);

			/// <summary>
			/// The gameObject of component, their children and their components will be destroyed.
			/// The gameObject(s) will be active and run in this frame, and it will be destroyed just end of the frame only.
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