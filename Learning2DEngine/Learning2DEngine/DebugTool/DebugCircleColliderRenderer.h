#pragma once

#include <glad/glad.h>
#include <vector>
#include <map>

#include "../System/Singleton.h"
#include "../Physics/CircleColliderComponent.h"

#include "DebugColliderRenderer.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		class DebugCircleColliderRenderer : public DebugColliderRenderer<Physics::CircleColliderComponent>, public System::Singleton<DebugCircleColliderRenderer>
		{
			friend class System::Singleton<DebugCircleColliderRenderer>;
		private:
			static constexpr const int CircleSegment = 50;
			static size_t minDynamicDataSize;

			DebugCircleColliderRenderer();

			void InitVao() override;
		public:
			void Draw(Render::RendererMode rendererMode, int layer) override;

			size_t GetMinDynamicDataSize() override;
			static void SetMinDynamicData(size_t dynamicDataSize);
		};
	}
}