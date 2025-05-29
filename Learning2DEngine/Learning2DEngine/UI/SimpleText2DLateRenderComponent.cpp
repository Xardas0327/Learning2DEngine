#include "SimpleText2DLateRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace UI
	{
		const std::string SimpleText2DLateRenderComponent::id = "L2DE_SimpleText2DLateRenderComponent";

		SimpleText2DLateRenderComponent::SimpleText2DLateRenderComponent(
			GameObject* gameObject,
			RendererMode mode,
			const FontSizePair& fontSizePair,
			int layer,
			glm::vec4 color)
			: RendererComponent(gameObject, mode, layer, fontSizePair, color),
			Component(gameObject)
		{
		}

		SimpleText2DLateRenderComponent::SimpleText2DLateRenderComponent(
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

		const std::string& SimpleText2DLateRenderComponent::GetId() const
		{
			return SimpleText2DLateRenderComponent::id;
		}

		SimpleText2DRenderer* SimpleText2DLateRenderComponent::GetInitedRenderer()
		{
			auto& renderer = SimpleText2DRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}

		void SimpleText2DLateRenderComponent::DestroyRenderer()
		{
			SimpleText2DRenderer::GetInstance().Destroy();
		}
	}
}