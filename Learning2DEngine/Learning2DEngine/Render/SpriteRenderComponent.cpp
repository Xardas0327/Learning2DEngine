#include "SpriteRenderComponent.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{
		const std::string SpriteRenderComponent::id = "L2DE_SpriteRenderComponent";
		int SpriteRenderComponent::refrenceNumber = 0;
		std::mutex SpriteRenderComponent::mutex;

		SpriteRenderComponent::SpriteRenderComponent(GameObject* gameObject, int layer, glm::vec4 color)
			: OldRendererComponent(gameObject, layer, color), Component(gameObject)
		{
		}

		SpriteRenderComponent::SpriteRenderComponent(GameObject* gameObject, const Texture2D& texture, int layer, glm::vec4 color)
			: OldRendererComponent(gameObject, layer, texture, color), Component(gameObject)
		{
		}

		void SpriteRenderComponent::Init()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				OldRendererComponent::Init();
				++SpriteRenderComponent::refrenceNumber;
			}
			else
			{
				OldRendererComponent::Init();
				++SpriteRenderComponent::refrenceNumber;
			}
		}

		void SpriteRenderComponent::Destroy()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				OldRendererComponent::Destroy();

				if (!(--SpriteRenderComponent::refrenceNumber))
				{
					MultiSpriteRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
			else
			{
				OldRendererComponent::Destroy();

				if (!(--SpriteRenderComponent::refrenceNumber))
				{
					MultiSpriteRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
		}

		const std::string& SpriteRenderComponent::GetId() const
		{
			return SpriteRenderComponent::id;
		}

		MultiSpriteRenderer* SpriteRenderComponent::GetRenderer() const
		{
			auto& renderer = MultiSpriteRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}
	}
}