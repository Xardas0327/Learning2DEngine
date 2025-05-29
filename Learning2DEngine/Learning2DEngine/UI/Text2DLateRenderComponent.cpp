#include "Text2DLateRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

    namespace UI
    {
		const std::string Text2DLateRenderComponent::id = "L2DE_Text2DLateRenderComponent";
		int Text2DLateRenderComponent::refrenceNumber = 0;
		std::mutex Text2DLateRenderComponent::mutex;

		Text2DLateRenderComponent::Text2DLateRenderComponent(GameObject* gameObject, const FontSizePair& fontSizePair, int layer, glm::vec4 color)
			: LateRendererComponent(gameObject, layer, fontSizePair, color),
			Component(gameObject)
		{
		}

		Text2DLateRenderComponent::Text2DLateRenderComponent(
			GameObject* gameObject,
			const FontSizePair& fontSizePair,
			const std::string& text,
			int layer,
			glm::vec4 color
		) : LateRendererComponent(gameObject, layer, fontSizePair, text, color),
			Component(gameObject)
		{

		}

		void Text2DLateRenderComponent::Init()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				LateRendererComponent::Init();
				++Text2DLateRenderComponent::refrenceNumber;
			}
			else
			{
				LateRendererComponent::Init();
				++Text2DLateRenderComponent::refrenceNumber;
			}
		}

		void Text2DLateRenderComponent::Destroy()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				LateRendererComponent::Destroy();

				if (!(--Text2DLateRenderComponent::refrenceNumber))
				{
					DestroyRenderer();
					componentManager.RemoveRenderer(RendererMode::LATERENDER, GetId());
				}
			}
			else
			{
				LateRendererComponent::Destroy();

				if (!(--Text2DLateRenderComponent::refrenceNumber))
				{
					DestroyRenderer();
					componentManager.RemoveRenderer(RendererMode::LATERENDER, GetId());
				}
			}
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