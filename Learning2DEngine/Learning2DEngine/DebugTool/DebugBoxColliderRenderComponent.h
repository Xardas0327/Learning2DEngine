#pragma once

#include "../Physics/BaseBoxColliderComponent.h"
#include "../Render/RendererComponent.h"

#include "DebugRenderData.h"
#include "DebugBoxColliderRenderer.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		class DebugBoxColliderRenderComponent : public Render::RendererComponent<DebugRenderData<Physics::BaseBoxColliderComponent>, DebugBoxColliderRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;
			/// <summary>
			/// It is counted, that how many DebugBoxColliderRenderComponent exist.
			/// </summary>
			static int refrenceNumber;
			static std::mutex mutex;

		protected:
			DebugBoxColliderRenderComponent(System::GameObject* gameObject, const Physics::BaseBoxColliderComponent* collider);

			void Init() override;
			void Destroy() override;

			const std::string& GetId() const override;

			DebugBoxColliderRenderer* GetRenderer() const override;
		};
	}
}