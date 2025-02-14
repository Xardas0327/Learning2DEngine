#pragma once

#include <vector>

#include "Transform.h"
#include "Component.h"

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
				return new GameObject(isActive);
			}

			static GameObject* Create(const Transform& transform, bool isActive = true)
			{
				return new GameObject(transform, isActive);
			}

			/// <summary>
			/// The gameObject and its components will be destroyed.
			/// </summary>
			/// <param name="gameObject"></param>
			static void Destroy(GameObject* gameObject)
			{
				delete gameObject;
			}

			/// <summary>
			/// The gameObject of component and its components will be destroyed.
			/// </summary>
			/// <param name="gameObject"></param>
			static void Destroy(Component* component)
			{
				delete component->gameObject;
			}
		};
	}
}