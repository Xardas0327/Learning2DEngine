#pragma once

#include "../Physics/CircleColliderComponent.h"
#include "../Render/RendererComponent.h"

#include "DebugRenderData.h"
#include "DebugCircleColliderRenderer.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		// It is used in Render Mode only
		// and it works well with renderers if the renderers use the camera view matrix.
		class DebugCircleColliderRenderComponent : public Render::RendererComponent<DebugRenderData<Physics::CircleColliderComponent>, DebugCircleColliderRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;

		protected:
			DebugCircleColliderRenderComponent(System::GameObject* gameObject, const Physics::CircleColliderComponent* collider);

			const std::string& GetId() const override;
			DebugCircleColliderRenderer* GetInitedRenderer() override;
			void DestroyRenderer() override;
		};
	}
}