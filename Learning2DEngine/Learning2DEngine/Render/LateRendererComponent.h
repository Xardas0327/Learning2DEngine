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
			template <class ...TRenderDataParams>
			LateRendererComponent(System::GameObject* gameObject, int layer = 0, TRenderDataParams... renderDataParams)
				: BaseRendererComponent<TRenderData, TRenderer>(gameObject, layer, renderDataParams...), System::Component(gameObject)
			{

			}

			/// <summary>
			/// If this function is override, it should care, that the renderer and renderdata will be added to the ComponentManager's LateRender.
			/// </summary>
			virtual void Init() override
			{
				auto& componentManager = System::ComponentManager::GetInstance();
				if (!componentManager.IsRendererExistInLateRender(this->GetId()))
				{
					componentManager.AddRendererToLateRender(this->GetId(), this->GetRenderer());
				}

				componentManager.AddDataToLateRender(this->GetId(), &this->data, this->GetLayer());
			}

			/// <summary>
			/// If this function is override, it should care, that the renderdata will be removed from the ComponentManager's LateRender.
			/// By default the renderer will not be removed automatically.
			/// </summary>
			virtual void Destroy() override
			{
				System::ComponentManager::GetInstance().RemoveDataFromLateRender(&this->data);
			}

		public:
			virtual void SetLayer(int value) override
			{
				BaseRendererComponent<TRenderData, TRenderer>::SetLayer(value);
				System::ComponentManager::GetInstance().ChangeLayerInLateRender(&this->data, this->GetLayer());
			}
		};
	}
}