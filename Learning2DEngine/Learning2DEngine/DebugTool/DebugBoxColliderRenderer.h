#pragma once

#include <glad/glad.h>
#include <vector>
#include <map>

#include "../System/Singleton.h"
#include "../Physics/BaseBoxColliderComponent.h"

#include "DebugColliderRenderer.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		class DebugBoxColliderRenderer : public DebugColliderRenderer<Physics::BaseBoxColliderComponent>, public System::Singleton<DebugBoxColliderRenderer>
		{
			friend class Singleton<DebugBoxColliderRenderer>;
		private:
			DebugBoxColliderRenderer();

			void InitVao() override;
		public:
			void Draw(int layer) override;
		};
	}
}