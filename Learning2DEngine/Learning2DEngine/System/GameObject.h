#pragma once

#include <list>

#include "Transform.h"
#include "Log.h"

namespace Learning2DEngine
{
	namespace System
	{
		class Renderer;
		class Behaviour;

		class GameObject final
		{
		private:
			std::list<Behaviour*> behaviours;
			Renderer* renderer;
		public:
			bool isActive;
			Transform transform;

			GameObject()
				: isActive(true), transform(), behaviours(), renderer(nullptr)
			{
			}

			GameObject(Transform transform)
				: isActive(true), transform(transform), behaviours(), renderer(nullptr)
			{
			}

			~GameObject()
			{
				for (const Behaviour* behaviour : behaviours)
				{
					if (behaviour != nullptr)
					{
						delete behaviour;
					}
				}

				if (renderer != nullptr)
				{
					//renderer->Destroy();
					delete renderer;
				}
			}

			template <class TBehaviour, class ...Params>
			TBehaviour* AddBehaviour(Params... params)
			{
				TBehaviour* behaviour = new TBehaviour(this, params);
				behaviours.push_back(behaviour);

				return behaviour;
			}

			template <class TBehaviour>
			TBehaviour* GetBehaviour()
			{
				TBehaviour* selectedBehaviour = nullptr;
				for (Behaviour* behaviour : behaviours)
				{
					selectedBehaviour = dynamic_cast<TBehaviour*>(behaviour);

					if (selectedBehaviour != nullptr)
						break;
				}

				return selectedBehaviour;
			}

			template <class TRenderer, class ...Params>
			TRenderer* AddRenderer(Params... params)
			{
				if (renderer != nullptr)
				{
					delete renderer;
					LOG_WARNING("GAMEOBJECT: The renderer was not null, that is why the previous renderer was deleted, before the new one was added.");
				}

				renderer = new TRenderer(this, params);
				//renderer->Init();

				return static_cast<TRenderer*>(renderer);
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
		/// Moreover, It is recommand, that the constructor of the inherited class is protected and
		/// only the GameObject can use this constructor.
		/// </summary>
		class Component
		{
		protected:
			friend class GameObject;

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

		/// <summary>
		/// The classes, which are inherited from Renderer,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Moreover, It is recommand, that the constructor of the inherited class is protected and
		/// only the GameObject can use this constructor.
		/// </summary>
		class Renderer : public virtual Component
		{
		protected:
			Renderer(System::GameObject* gameObject)
				: Component(gameObject)
			{

			}
		public:
			virtual void Init() = 0;
			virtual void Destroy() = 0;

			virtual void Draw() = 0;
		};

		/// <summary>
		/// The classes, which are inherited from Renderer,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Moreover, It is recommand, that the constructor of the inherited class is protected and
		/// only the GameObject can use this constructor.
		/// </summary>
		class Behaviour : public virtual Component
		{
		protected:
			Behaviour(System::GameObject* gameObject)
				: Component(gameObject)
			{

			}
		};
	}
}