#pragma once

#include <vector>

#include "Transform.h"
#include "Log.h"
#include "Component.h"
#include "../Render/Renderer.h"

namespace Learning2DEngine
{
	namespace System
	{
		class GameObject final
		{
		private:
			std::vector<Component*> components;
		public:
			bool isActive;
			Transform transform;

			GameObject(bool isActive = true)
				: isActive(isActive), transform(), components()
			{
			}

			GameObject(Transform transform, bool isActive = true)
				: isActive(isActive), transform(transform), components()
			{
			}

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

			/// <summary>
			/// The gameObject and its behaviours and its renderer will be destroyed.
			/// </summary>
			/// <param name="gameObject"></param>
			static void Destroy(GameObject* gameObject)
			{
				delete gameObject;
			}

			/// <summary>
			/// The gameObject of component and its behaviours and its renderer will be destroyed.
			/// </summary>
			/// <param name="gameObject"></param>
			static void Destroy(Component* component)
			{
				delete component->gameObject;
			}
		};
	}
}