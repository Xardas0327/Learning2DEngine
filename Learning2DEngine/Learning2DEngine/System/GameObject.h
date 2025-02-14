#pragma once

#include <vector>

#include "Transform.h"
#include "Component.h"
#include "GameObjectManager.h"

namespace Learning2DEngine
{
	namespace System
	{
		class GameObject final
		{
		private:
			std::vector<Component*> components;

			GameObject(bool isActive = true)
				: isActive(isActive), transform(), components()
			{
			}

			GameObject(const Transform& transform, bool isActive = true)
				: isActive(isActive), transform(transform), components()
			{
			}
		public:
			bool isActive;
			Transform transform;

			~GameObject()
			{
				int a = 1;
				for (Component* component : components)
				{
					if (component != nullptr)
					{
						component->Destroy();
						delete component;
					}
				}
			}

			template <class TComponent, class ...Params>
			TComponent* AddComponent(Params... params)
			{
				TComponent* component = new TComponent(this, params...);
				components.push_back(component);
				component->Init();

				return component;
			}

			template <class TComponent>
			TComponent* GetComponent()
			{
				TComponent* selectedComponent = nullptr;
				for (Component* component : components)
				{
					selectedComponent = dynamic_cast<TComponent*>(component);

					if (selectedComponent != nullptr)
						break;
				}

				return selectedComponent;
			}

			template <class TComponent>
			const TComponent* GetComponent() const
			{
				TComponent* selectedComponent = nullptr;
				for (Component* component : components)
				{
					selectedComponent = dynamic_cast<TComponent*>(component);

					if (selectedComponent != nullptr)
						break;
				}

				return selectedComponent;
			}

			template <class TComponent>
			std::vector<TComponent*> GetComponents()
			{
				std::vector<TComponent*> selectedComponents;
				for (Component* component : components)
				{
					TComponent* selectedComponent = dynamic_cast<TComponent*>(component);

					if (selectedComponent != nullptr)
					{
						selectedComponents.push_back(selectedComponent);
					}
				}

				return selectedComponents;
			}

			static GameObject* Create(bool isActive = true)
			{
				auto gameObject = new GameObject(isActive);
				GameObjectManager::GetInstance().Add(gameObject);
				return gameObject;
			}

			static GameObject* Create(const Transform& transform, bool isActive = true)
			{
				auto gameObject = new GameObject(transform, isActive);
				GameObjectManager::GetInstance().Add(gameObject);
				return gameObject;
			}

			/// <summary>
			/// The gameObject and its components will be destroyed.
			/// It will be inactive immediately, but it will be destroyed just end of the frame only.
			/// </summary>
			/// <param name="gameObject"></param>
			static void Destroy(GameObject* gameObject)
			{
				gameObject->isActive = false;
				GameObjectManager::GetInstance().MarkForDestroy(gameObject);
			}

			/// <summary>
			/// The gameObject of component and its components will be destroyed.
			/// It will be inactive immediately, but it will be destroyed just end of the frame only.
			/// </summary>
			/// <param name="gameObject"></param>
			static void Destroy(Component* component)
			{
				component->gameObject->isActive = false;
				GameObjectManager::GetInstance().MarkForDestroy(component->gameObject);
			}
		};
	}
}