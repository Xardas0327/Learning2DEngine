#include "SimpleSpriteRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{
		const std::string SimpleSpriteRenderComponent::id = "L2DE_SimpleSpriteRenderComponent";
		int SimpleSpriteRenderComponent::refrenceNumber = 0;
		std::mutex SimpleSpriteRenderComponent::mutex;

		SimpleSpriteRenderComponent::SimpleSpriteRenderComponent(GameObject* gameObject, int layer, glm::vec4 color)
			: RendererComponent(gameObject, layer, color), BaseRendererComponent(gameObject, layer, color), Component(gameObject)
		{
		}

		SimpleSpriteRenderComponent::SimpleSpriteRenderComponent(GameObject* gameObject, const Texture2D& texture, int layer, glm::vec4 color)
			: RendererComponent(gameObject, layer, texture, color), BaseRendererComponent(gameObject, layer, texture, color), Component(gameObject)
		{
		}

		void SimpleSpriteRenderComponent::Init()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				RendererComponent::Init();
				++SimpleSpriteRenderComponent::refrenceNumber;
			}
			else
			{
				RendererComponent::Init();
				++SimpleSpriteRenderComponent::refrenceNumber;
			}
		}

		void SimpleSpriteRenderComponent::Destroy()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				RendererComponent::Destroy();

				if (!(--SimpleSpriteRenderComponent::refrenceNumber))
				{
					SimpleSpriteRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
			else
			{
				RendererComponent::Destroy();

				if (!(--SimpleSpriteRenderComponent::refrenceNumber))
				{
					SimpleSpriteRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
		}

		const std::string& SimpleSpriteRenderComponent::GetId() const
		{
			return SimpleSpriteRenderComponent::id;
		}

		SimpleSpriteRenderer* SimpleSpriteRenderComponent::GetRenderer() const
		{
			auto& renderer = SimpleSpriteRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}
	}
}