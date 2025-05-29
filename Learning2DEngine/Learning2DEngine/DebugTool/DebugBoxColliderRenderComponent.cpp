#include "DebugBoxColliderRenderComponent.h"

#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;
	using namespace Physics;

	namespace DebugTool
	{
		const std::string DebugBoxColliderRenderComponent::id = "L2DE_DebugBoxColliderRenderComponent";
		int DebugBoxColliderRenderComponent::refrenceNumber = 0;
		std::mutex DebugBoxColliderRenderComponent::mutex;

		DebugBoxColliderRenderComponent::DebugBoxColliderRenderComponent(GameObject* gameObject, const BaseBoxColliderComponent* collider)
			: OldRendererComponent(gameObject, L2DE_DEBUG_SHOW_COLLIDER_DEFAULT_LAYER, collider), Component(gameObject)
		{
		}

		void DebugBoxColliderRenderComponent::Init()
		{
			auto& componentManager = ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				OldRendererComponent::Init();
				++DebugBoxColliderRenderComponent::refrenceNumber;
			}
			else
			{
				OldRendererComponent::Init();
				++DebugBoxColliderRenderComponent::refrenceNumber;
			}
		}

		void DebugBoxColliderRenderComponent::Destroy()
		{
			auto& componentManager = System::ComponentManager::GetInstance();
			if (componentManager.GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(mutex);
				OldRendererComponent::Destroy();

				if (!(--DebugBoxColliderRenderComponent::refrenceNumber))
				{
					DebugBoxColliderRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
			else
			{
				OldRendererComponent::Destroy();

				if (!(--DebugBoxColliderRenderComponent::refrenceNumber))
				{
					DebugBoxColliderRenderer::GetInstance().Destroy();
					componentManager.RemoveRendererFromRender(GetId());
				}
			}
		}

		const std::string& DebugBoxColliderRenderComponent::GetId() const
		{
			return DebugBoxColliderRenderComponent::id;
		}

		DebugBoxColliderRenderer* DebugBoxColliderRenderComponent::GetRenderer() const
		{
			auto& renderer = DebugBoxColliderRenderer::GetInstance();
			renderer.Init();

			return &renderer;
		}
	}
}