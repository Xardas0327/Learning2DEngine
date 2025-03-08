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
		/// The TRenderData should be a class, which is inhereted from RenderData.
		/// The TRenderer should be a class, which is inhereted from IRenderer.
		/// Please check for more info about `System::Component` and `BaseRendererComponent`.
		/// </summary>
		template<class TRenderData, class TRenderer>
		class LateRendererComponent : public virtual BaseRendererComponent<TRenderData, TRenderer>
		{
		protected:
			LateRendererComponent(System::GameObject* gameObject, int layer = 0)
				: BaseRendererComponent(gameObject, layer)
			{

			}

			/// <summary>
			/// If this function is override, it should call the LateRendererComponent::Init() in the first line.
			/// </summary>
			virtual void Init() override
			{
				//System::ComponentManager::GetInstance().AddToRenderer(this);

				//if ComponentManager's LateRenderer doesn't have TRenderer add it with Id
				//and the TRenderData should be added always.
			}

			/// <summary>
			/// If this function is override, it should call the LateRendererComponent::Destroy() in the first line.
			/// </summary>
			virtual void Destroy() override
			{
				//System::ComponentManager::GetInstance().RemoveFromRenderer(this);

				// Remove the TRenderData from ComponentManager's LateRenderer.
				// The TRenderer removing should be automatically.
			}

		public:
			virtual void SetLayer(int value) override
			{
				BaseRendererComponent::SetLayer(value);
				//System::ComponentManager::GetInstance().NeedReorderRenderers();

				// This should trigger a reordering
			}
		};
	}
}