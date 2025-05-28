#pragma once

#include <glad/glad.h>
#include <vector>
#include <map>

#include "../System/Singleton.h"
#include "../Physics/BaseCircleColliderComponent.h"

#include "DebugColliderRenderer.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
#define L2DE_DEBUG_CIRCLE_SEGMENT 50

		class DebugCircleColliderRenderer : public DebugColliderRenderer<Physics::BaseCircleColliderComponent>, public System::Singleton<DebugCircleColliderRenderer>
		{
			friend class Singleton<DebugCircleColliderRenderer>;
		private:
			DebugCircleColliderRenderer();

			void InitVao() override;
		public:
			void Draw(int layer) override;
		};
	}
}