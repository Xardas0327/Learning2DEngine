#pragma once

#include "../Physics/BaseCircleColliderComponent.h"
#include "../Render/OldRendererComponent.h"

#include "DebugRenderData.h"
#include "DebugCircleColliderRenderer.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		class DebugCircleColliderRenderComponent : public Render::OldRendererComponent<DebugRenderData<Physics::BaseCircleColliderComponent>, DebugCircleColliderRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;
			/// <summary>
			/// It is counted, that how many DebugCircleColliderRenderComponent exist.
			/// </summary>
			static int refrenceNumber;
			static std::mutex mutex;

		protected:
			DebugCircleColliderRenderComponent(System::GameObject* gameObject, const Physics::BaseCircleColliderComponent* collider);

			void Init() override;
			void Destroy() override;

			const std::string& GetId() const override;

			DebugCircleColliderRenderer* GetRenderer() const override;
		};
	}
}