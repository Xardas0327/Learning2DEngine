#pragma once

#include "OldBaseRendererComponent.h"
#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from RendererComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Please check for more info about `System::Component` and `BaseRendererComponent`.
		/// </summary>
		class OldRendererComponent : public virtual OldBaseRendererComponent
		{
			//friend class System::GameObject;
		protected:
			OldRendererComponent(System::GameObject* gameObject)
				: OldBaseRendererComponent(gameObject)
			{

			}

			OldRendererComponent(System::GameObject* gameObject, int layer)
				: OldBaseRendererComponent(gameObject, layer)
			{

			}

			/// <summary>
			/// If this function is override, it must call the RendererComponent::Init() in the first line.
			/// </summary>
			virtual void Init() override
			{
				System::ComponentManager::GetInstance().AddToRenderer(this);
			}

			/// <summary>
			/// If this function is override, it must call the RendererComponent::Destroy() in the first line.
			/// </summary>
			virtual void Destroy() override
			{
				System::ComponentManager::GetInstance().RemoveFromRenderer(this);
			}

		public:
			virtual void SetLayer(int value) override
			{
				OldBaseRendererComponent::SetLayer(value);
				System::ComponentManager::GetInstance().NeedReorderRenderers();
			}
		};
	}
}