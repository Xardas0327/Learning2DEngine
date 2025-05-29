#pragma once

#include "BaseRendererComponent.h"
#include "RendererMode.h"

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
			RendererComponent(System::GameObject* gameObject, RendererMode mode, int layer = 0, TRenderDataParams&&... renderDataParams)
				: BaseRendererComponent<TRenderData, TRenderer>(gameObject, layer, std::forward<TRenderDataParams>(renderDataParams)...),
				System::Component(gameObject), mode(mode)
			{

			}

		public:
			const RendererMode mode;
		};
	}
}