#pragma once

#include <glad/glad.h>
#include <vector>
#include <map>

#include "../System/Singleton.h"
#include "../Render/IRenderer.h"
#include "../Render/Shader.h"
#include "../Render/RenderData.h"
#include "../Render/BaseColorDynamicData.h"
#include "../Physics/BaseCircleColliderComponent.h"

#include "DebugRenderData.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
#define L2DE_DEBUG_CIRCLE_SEGMENT 50

		class DebugCircleColliderRenderer : public Render::IRenderer, public System::Singleton<DebugCircleColliderRenderer>
		{
			friend class Singleton<DebugCircleColliderRenderer>;
		private:
			Render::Shader shader;
			GLuint vao;
			GLuint vboStatic;
			GLuint vboDynamic;
			unsigned int maxObjectSize;

			std::map<int, std::vector<DebugRenderData<Physics::BaseCircleColliderComponent>*>> debugRenderData;
			Render::BaseColorDynamicData* dynamicData;

			DebugCircleColliderRenderer();

			void InitShader();
			void InitVao();
			void DestroyObject();
		public:
			void Init() override;
			void Destroy() override;
			void SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData) override;
			void Draw(int layer) override;
		};
	}
}