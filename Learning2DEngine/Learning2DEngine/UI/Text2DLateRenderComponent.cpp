#include "Text2DLateRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;

    namespace UI
    {
		const std::string Text2DLateRenderComponent::id = "L2DE_Text2DLateRenderComponent";
		int Text2DLateRenderComponent::refrenceNumber = 0;

		Text2DLateRenderComponent::Text2DLateRenderComponent(GameObject* gameObject, const FontSizePair& fontSizePair, int layer, glm::vec4 color)
			: LateRendererComponent(gameObject, layer), BaseRendererComponent(gameObject, layer), Component(gameObject), mutex()
		{
			data.fontSizePair = fontSizePair;
			data.color = color;
		}

		Text2DLateRenderComponent::Text2DLateRenderComponent(GameObject* gameObject, const FontSizePair& fontSizePair, std::string text, int layer, glm::vec4 color)
			: LateRendererComponent(gameObject, layer), BaseRendererComponent(gameObject, layer), Component(gameObject), mutex()
		{
			data.fontSizePair = fontSizePair;
			data.text = text;
			data.color = color;
		}

		void Text2DLateRenderComponent::Init()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				LateRendererComponent::Init();
				++refrenceNumber;
			}
			else
			{
				LateRendererComponent::Init();
				++refrenceNumber;
			}
		}

		void Text2DLateRenderComponent::Destroy()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				LateRendererComponent::Destroy();

				if (!(--refrenceNumber))
				{
					Text2DRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
			else
			{
				LateRendererComponent::Destroy();

				if (!(--refrenceNumber))
				{
					Text2DRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
		}

		const std::string& Text2DLateRenderComponent::GetId() const
		{
			return Text2DLateRenderComponent::id;
		}

		Text2DRenderer* Text2DLateRenderComponent::GetRenderer() const
		{
			auto& renderer = Text2DRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}
    }
}