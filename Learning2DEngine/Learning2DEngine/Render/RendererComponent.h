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
		/// The TRenderData should be a class, which is inhereted from RenderData.
		/// The TRenderer should be a class, which is inhereted from IRenderer.
		/// Please check for more info about `System::Component` and `BaseRendererComponent`.
		/// </summary>
		template<class TRenderData, class TRenderer>
		class RendererComponent : public BaseRendererComponent<TRenderData, TRenderer>
		{
		protected:
			template <class ...TRenderDataParams>
			RendererComponent(System::GameObject* gameObject, int layer = 0, TRenderDataParams&&... renderDataParams)
				: BaseRendererComponent<TRenderData, TRenderer>(gameObject, layer, std::forward<TRenderDataParams>(renderDataParams)...),
				System::Component(gameObject)
			{

			}

			/// <summary>
			/// If this function is override, it should care, that the renderer and renderdata will be added to the ComponentManager's Render.
			/// </summary>
			virtual void Init() override
			{
				auto& componentManager = System::ComponentManager::GetInstance();
				if (!componentManager.IsRendererExistInRender(this->GetId()))
				{
					componentManager.AddRendererToRender(this->GetId(), this->GetRenderer());
				}

				componentManager.AddDataToRender(this->GetId(), &this->data, this->GetLayer());
			}

			/// <summary>
			/// If this function is override, it should care, that the renderdata will be removed from the ComponentManager's Render.
			/// By default the renderer will not be removed automatically.
			/// </summary>
			virtual void Destroy() override
			{
				System::ComponentManager::GetInstance().RemoveDataFromRender(&this->data);
			}

		public:
			virtual void SetLayer(int value) override
			{
				BaseRendererComponent<TRenderData, TRenderer>::SetLayer(value);
				System::ComponentManager::GetInstance().ChangeLayerInRender(&this->data, this->GetLayer());
			}
		};
	}
}