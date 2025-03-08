#pragma once

#include "BaseRendererComponent.h"
#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from RendererComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// The TRenderData should be a class, which is inhereted from IRenderData.
		/// The TRenderer should be a class, which is inhereted from IRenderer.
		/// Please check for more info about `System::Component` and `BaseRendererComponent`.
		/// </summary>
		template<class TRenderData, class TRenderer>
		class RendererComponent : public virtual BaseRendererComponent<TRenderData, TRenderer>
		{
		protected:
			RendererComponent(System::GameObject* gameObject, int layer = 0)
				: BaseRendererComponent(gameObject, layer)
			{

			}

			RendererComponent(System::GameObject* gameObject, const TRenderData& data, int layer = 0)
				: BaseRendererComponent(gameObject, data, layer)
			{

			}

			/// <summary>
			/// If this function is override, it should call the RendererComponent::Init() in the first line.
			/// </summary>
			virtual void Init() override
			{
				//System::ComponentManager::GetInstance().AddToRenderer(this);

				//if ComponentManager's Renderer doesn't have TRenderer add it with Id
				//and the TRenderData should be added always.
			}

			/// <summary>
			/// If this function is override, it should call the RendererComponent::Destroy() in the first line.
			/// </summary>
			virtual void Destroy() override
			{
				//System::ComponentManager::GetInstance().RemoveFromRenderer(this);

				// Remove the TRenderData from ComponentManager's Renderer.
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