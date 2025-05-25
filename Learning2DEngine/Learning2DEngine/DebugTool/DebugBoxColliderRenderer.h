#pragma once

#include <glad/glad.h>
#include <vector>
#include <map>

#include "../System/Singleton.h"
#include "../Render/IRenderer.h"
#include "../Render/Shader.h"
#include "../Render/RenderData.h"
#include "../Render/BaseColorDynamicData.h"
#include "../Physics/BaseBoxColliderComponent.h"

#include "DebugRenderData.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		class DebugBoxColliderRenderer : public Render::IRenderer, public System::Singleton<DebugBoxColliderRenderer>
		{
			friend class Singleton<DebugBoxColliderRenderer>;
		private:
			Render::Shader shader;
			GLuint vao;
			GLuint vboStatic;
			GLuint vboDynamic;
			size_t maxObjectSize;

			std::map<int, std::vector<DebugRenderData<Physics::BaseBoxColliderComponent>*>> debugRenderData;
			Render::BaseColorDynamicData* dynamicData;

			DebugBoxColliderRenderer();

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