#pragma once

#include <vector>

#include "Transform.h"
#include "Log.h"
#include "Component.h"
#include "Behaviour.h"
#include "../Render/Renderer.h"

namespace Learning2DEngine
{
	namespace System
	{
		class GameObject final
		{
		private:
			std::vector<Behaviour*> behaviours;
			Render::Renderer* renderer;
		public:
			bool isActive;
			Transform transform;

			GameObject(bool isActive = true)
				: isActive(isActive), transform(), behaviours(), renderer(nullptr)
			{
			}

			GameObject(Transform transform, bool isActive = true)
				: isActive(isActive), transform(transform), behaviours(), renderer(nullptr)
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
					renderer->Destroy();
					delete renderer;
				}
			}

			template <class TBehaviour, class ...Params>
			TBehaviour* AddBehaviour(Params... params)
			{
				TBehaviour* behaviour = new TBehaviour(this, params...);
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


				TRenderer* newRenderer = new TRenderer(this, params...);
				renderer = newRenderer;
				renderer->Init();

				return newRenderer;
			}

			template <class TRenderer>
			TRenderer* GetRenderer()
			{
				if (renderer == nullptr)
					return nullptr;

				TRenderer* selectedRenderer = dynamic_cast<TRenderer*>(renderer);

				return selectedRenderer;
			}

			inline Render::Renderer* GetRenderer()
			{
				return renderer;
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