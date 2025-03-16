#include "SpriteRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{
		const std::string SpriteRenderComponent::id = "L2DE_SpriteRenderComponent";
		int SpriteRenderComponent::refrenceNumber = 0;

		SpriteRenderComponent::SpriteRenderComponent(GameObject* gameObject, int layer, glm::vec4 color)
			: RendererComponent(gameObject, layer, color), BaseRendererComponent(gameObject, layer, color), Component(gameObject), mutex()
		{
		}

		SpriteRenderComponent::SpriteRenderComponent(GameObject* gameObject, const Texture2D& texture, int layer, glm::vec4 color)
			: RendererComponent(gameObject, layer, texture, color), BaseRendererComponent(gameObject, layer, texture, color), Component(gameObject), mutex()
		{
		}

		void SpriteRenderComponent::Init()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				RendererComponent::Init();
				++SpriteRenderComponent::refrenceNumber;
			}
			else
			{
				RendererComponent::Init();
				++SpriteRenderComponent::refrenceNumber;
			}
		}

		void SpriteRenderComponent::Destroy()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				RendererComponent::Destroy();

				if (!(--SpriteRenderComponent::refrenceNumber))
				{
					SpriteRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
			else
			{
				RendererComponent::Destroy();

				if (!(--SpriteRenderComponent::refrenceNumber))
				{
					SpriteRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
		}

		const std::string& SpriteRenderComponent::GetId() const
		{
			return SpriteRenderComponent::id;
		}

		SpriteRenderer* SpriteRenderComponent::GetRenderer() const
		{
			auto& renderer = SpriteRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}
	}
}