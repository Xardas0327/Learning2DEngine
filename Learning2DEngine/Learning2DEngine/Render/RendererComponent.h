#pragma once

#include "BaseRendererComponent.h"
#include "../System/ObjectManager.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from RendererComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Please check for more info about `System::Component` and `BaseRendererComponent`.
		/// </summary>
		class RendererComponent : public virtual BaseRendererComponent
		{
			friend class System::GameObject;
		protected:
			RendererComponent(System::GameObject* gameObject)
				: BaseRendererComponent(gameObject)
			{

			}

			RendererComponent(System::GameObject* gameObject, int layer)
				: BaseRendererComponent(gameObject, layer)
			{

			}

			/// <summary>
			/// If this function is override, it must call the RendererComponent::Init() in the first line.
			/// </summary>
			virtual void Init() override
			{
				System::ObjectManager::GetInstance().AddToRenderer(this);
			}

			/// <summary>
			/// If this function is override, it must call the RendererComponent::Destroy() in the first line.
			/// </summary>
			virtual void Destroy() override
			{
				System::ObjectManager::GetInstance().RemoveFromRenderer(this);
			}

		public:
			virtual void SetLayer(int value) override
			{
				BaseRendererComponent::SetLayer(value);
				System::ObjectManager::GetInstance().NeedReorderRenderers();
			}
		};
	}
}