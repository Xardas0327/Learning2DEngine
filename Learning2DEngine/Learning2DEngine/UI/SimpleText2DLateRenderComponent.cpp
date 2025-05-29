#include "SimpleText2DLateRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace UI
	{
		const std::string SimpleText2DLateRenderComponent::id = "L2DE_SimpleText2DLateRenderComponent";
		int SimpleText2DLateRenderComponent::refrenceNumber = 0;
		std::mutex SimpleText2DLateRenderComponent::mutex;

		SimpleText2DLateRenderComponent::SimpleText2DLateRenderComponent(GameObject* gameObject, const FontSizePair& fontSizePair, int layer, glm::vec4 color)
			: LateRendererComponent(gameObject, layer, fontSizePair, color),
			Component(gameObject)
		{
		}

		SimpleText2DLateRenderComponent::SimpleText2DLateRenderComponent(
			GameObject* gameObject,
			const FontSizePair& fontSizePair,
			const std::string& text,
			int layer,
			glm::vec4 color
		) : LateRendererComponent(gameObject, layer, fontSizePair, text, color),
			Component(gameObject)
		{

		}

		void SimpleText2DLateRenderComponent::Init()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				LateRendererComponent::Init();
				++SimpleText2DLateRenderComponent::refrenceNumber;
			}
			else
			{
				LateRendererComponent::Init();
				++SimpleText2DLateRenderComponent::refrenceNumber;
			}
		}

		void SimpleText2DLateRenderComponent::Destroy()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				LateRendererComponent::Destroy();

				if (!(--SimpleText2DLateRenderComponent::refrenceNumber))
				{
					SimpleText2DRenderer::GetInstance().Destroy();
					componentManager.RemoveRenderer(RendererMode::LATERENDER, GetId());
				}
			}
			else
			{
				LateRendererComponent::Destroy();

				if (!(--SimpleText2DLateRenderComponent::refrenceNumber))
				{
					SimpleText2DRenderer::GetInstance().Destroy();
					componentManager.RemoveRenderer(RendererMode::LATERENDER, GetId());
				}
			}
		}

		const std::string& SimpleText2DLateRenderComponent::GetId() const
		{
			return SimpleText2DLateRenderComponent::id;
		}

		SimpleText2DRenderer* SimpleText2DLateRenderComponent::GetRenderer() const
		{
			auto& renderer = SimpleText2DRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}
	}
}