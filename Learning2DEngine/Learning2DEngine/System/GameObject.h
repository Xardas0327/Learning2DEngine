#pragma once

#include <list>

#include "Transform.h"
#include "Log.h"
#include "../Render/Renderer.h"

namespace Learning2DEngine
{
	namespace System
	{
		class Component;

		class GameObject final
		{
		private:
			std::list<Component*> components;
			Render::Renderer* renderer;
		public:
			bool isActive;
			Transform transform;

			GameObject()
				: isActive(true), transform(), components(), renderer(nullptr)
			{
			}

			GameObject(Transform transform)
				: isActive(true), transform(transform), components(), renderer(nullptr)
			{
			}

			~GameObject()
			{
				for (const Component* component : components)
				{
					if (component != nullptr)
					{
						delete component;
					}
				}

				if (renderer != nullptr)
				{
					delete renderer;
				}
			}

			template <class TComponent, class ...Params>
			TComponent* AddComponent(Params... params)
			{
				TComponent* component = new TComponent(this, params);
				components.push_back(component);

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

			template <class TRenderer, class ...Params>
			TRenderer* AddRenderer(Params... params)
			{
				if (renderer != nullptr)
				{
					delete renderer;
					LOG_WARNING("GAMEOBJECT: The renderer was not null, that is why the previous renderer was deleted, before the new one was added.")
				}

				TComponent* component = new TComponent(this, params);
				components.push_back(component);

				return component;
			}

			template <class TRenderer>
			TRenderer* GetRenderer()
			{
				if (renderer == nullptr)
					return nullptr;

				TRenderer* selectedRenderer = dynamic_cast<TRenderer*>(renderer);

				return selectedRenderer;
			}
		};

		/// <summary>
		/// The classes, which are inherited from Component,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Moreover, It is recommand, that the constructor of the inherited class is private or protected and
		/// only the GameObject can use this constructor.
		/// </summary>
		class Component
		{
			friend class GameObject;
		protected:
			bool isActive;

			Component(GameObject* gameObject)
				: gameObject(gameObject), isActive(true)
			{

			}
		public:
			GameObject* const gameObject;

			inline void SetActive(bool value)
			{
				isActive = value;
			}

			inline bool GetActive()
			{
				return isActive;
			}

			/// <summary>
			/// The Component and its GameObject are active or not.
			/// </summary>
			/// <returns></returns>
			inline bool GetRealActive()
			{
				return (gameObject != nullptr
					&& gameObject->isActive && isActive);
			}
		};
	}
}