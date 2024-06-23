#pragma once

#include <list>

#include "Transform.h"

namespace Learning2DEngine
{
	namespace System
	{
		class Component;

		class GameObject
		{
		private:
			std::list<Component*> components;
		public:
			bool isActive;
			Transform transform;

			GameObject()
				: isActive(true), transform(), components()
			{
			}

			GameObject(Transform transform)
				: isActive(true), transform(transform), components()
			{

			}

			~GameObject()
			{
				for (const Component* component : components)
				{
					delete component;
				}
			}

			template <class TComponent, class ...Params>
			TComponent* AddComponent(Params... params)
			{
				TComponent* component = new TComponent(params);
				components.push_back(component);

				return component;
			}

			template <class TComponent>
			TComponent* GetComponent()
			{
				TComponent* selectedComponent = nullptr;
				for (const Component* component : components)
				{
					selectedComponent = dynamic_cast<TComponent*>(component);

					if (selectedComponent != nullptr)
						break;
				}

				return selectedComponent;
			}
		};

		class Component
		{
			friend class GameObject;
		private:
			bool isActive;
			GameObject* gameObject;

			Component(GameObject* gameObject)
				: gameObject(gameObject), isActive(true)
			{

			}
		public:
			inline GameObject* GetGameObject()
			{
				return gameObject;
			}

			inline void SetActive(bool value)
			{
				isActive = value;
			}

			inline bool GetActive()
			{
				return isActive;
			}

			/// <summary>
			/// It check its GameObject too.
			/// </summary>
			/// <returns></returns>
			inline bool GetRealActive()
			{
				return gameObject != nullptr
					&& gameObject->isActive && isActive;
			}
		};
	}
}