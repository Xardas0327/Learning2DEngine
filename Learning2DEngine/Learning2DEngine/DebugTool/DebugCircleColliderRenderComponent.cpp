#include "DebugCircleColliderRenderComponent.h"

#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;
	using namespace Physics;

	namespace DebugTool
	{
		const std::string DebugCircleColliderRenderComponent::id = "L2DE_DebugCircleColliderRenderComponent";
		int DebugCircleColliderRenderComponent::refrenceNumber = 0;
		std::mutex DebugCircleColliderRenderComponent::mutex;

		DebugCircleColliderRenderComponent::DebugCircleColliderRenderComponent(GameObject* gameObject, BaseCircleColliderComponent* collider)
			: RendererComponent(gameObject, L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_LAYER, collider),
			BaseRendererComponent(gameObject, L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_LAYER, collider), Component(gameObject)
		{
		}

		void DebugCircleColliderRenderComponent::Init()
		{
			auto& componentManager = ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				RendererComponent::Init();
				++DebugCircleColliderRenderComponent::refrenceNumber;
			}
			else
			{
				RendererComponent::Init();
				++DebugCircleColliderRenderComponent::refrenceNumber;
			}
		}

		void DebugCircleColliderRenderComponent::Destroy()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				RendererComponent::Destroy();

				if (!(--DebugCircleColliderRenderComponent::refrenceNumber))
				{
					DebugCircleColliderRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
			else
			{
				RendererComponent::Destroy();

				if (!(--DebugCircleColliderRenderComponent::refrenceNumber))
				{
					DebugCircleColliderRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
		}

		const std::string& DebugCircleColliderRenderComponent::GetId() const
		{
			return DebugCircleColliderRenderComponent::id;
		}

		DebugCircleColliderRenderer* DebugCircleColliderRenderComponent::GetRenderer() const
		{
			auto& renderer = DebugCircleColliderRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}
	}
}