#include "Text2DLateRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

    namespace UI
    {
		const std::string Text2DLateRenderComponent::id = "L2DE_Text2DLateRenderComponent";

		Text2DLateRenderComponent::Text2DLateRenderComponent(
			GameObject* gameObject,
			RendererMode mode,
			const FontSizePair& fontSizePair,
			int layer,
			glm::vec4 color)
			: RendererComponent(gameObject, mode, layer, fontSizePair, color),
			Component(gameObject)
		{
		}

		Text2DLateRenderComponent::Text2DLateRenderComponent(
			GameObject* gameObject,
			RendererMode mode,
			const FontSizePair& fontSizePair,
			const std::string& text,
			int layer,
			glm::vec4 color
		) : RendererComponent(gameObject, mode, layer, fontSizePair, text, color),
			Component(gameObject)
		{

		}

		const std::string& Text2DLateRenderComponent::GetId() const
		{
			return Text2DLateRenderComponent::id;
		}

		MultiText2DRenderer* Text2DLateRenderComponent::GetInitedRenderer()
		{
			auto& renderer = MultiText2DRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}

		void Text2DLateRenderComponent::DestroyRenderer()
		{
			MultiText2DRenderer::GetInstance().Destroy();
		}
    }
}