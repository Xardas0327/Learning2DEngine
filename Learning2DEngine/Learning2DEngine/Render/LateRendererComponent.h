#pragma once

#include "BaseRendererComponent.h"
#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from LateRendererComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Please check more info about `System::Component` and `BaseRendererComponent`.
		/// </summary>
		class LateRendererComponent : public virtual BaseRendererComponent
		{
			friend class System::GameObject;
		protected:
			LateRendererComponent(System::GameObject* gameObject)
				: BaseRendererComponent(gameObject)
			{

			}

			LateRendererComponent(System::GameObject* gameObject, int layer)
				: BaseRendererComponent(gameObject, layer)
			{

			}

			/// <summary>
			/// If this function is override, it must call the LateRendererComponent::Init() in the first line.
			/// </summary>
			virtual void Init() override
			{
				System::ComponentManager::GetInstance().AddToLateRenderer(this);
			}

			/// <summary>
			/// If this function is override, it must call the LateRendererComponent::Destroy() in the first line.
			/// </summary>
			virtual void Destroy() override
			{
				System::ComponentManager::GetInstance().RemoveFromLateRenderer(this);
			}

		public:
			virtual void SetLayer(int value) override
			{
				BaseRendererComponent::SetLayer(value);
				System::ComponentManager::GetInstance().NeedReorderLateRenderers();
			}
		};
	}
}