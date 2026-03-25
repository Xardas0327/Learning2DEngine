#pragma once

#include <glad/glad.h>
#include <vector>
#include <map>

#include "../System/Singleton.h"
#include "../Physics/BoxColliderComponent.h"

#include "DebugColliderRenderer.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		class DebugBoxColliderRenderer : public DebugColliderRenderer<Physics::BoxColliderComponent>, public System::Singleton<DebugBoxColliderRenderer>
		{
			friend class System::Singleton<DebugBoxColliderRenderer>;
		private:
			static size_t minDynamicDataSize;

			DebugBoxColliderRenderer();

			void InitVao() override;
		public:
			void Draw(Render::RendererMode rendererMode, int layer) override;

			size_t GetMinDynamicDataSize() override;
			static void SetMinDynamicData(size_t dynamicDataSize);
		};
	}
}