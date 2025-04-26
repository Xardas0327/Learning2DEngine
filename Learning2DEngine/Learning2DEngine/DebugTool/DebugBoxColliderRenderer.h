#pragma once

#include <vector>
#include <map>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "../Render/IRenderer.h"
#include "../Render/Shader.h"
#include "../Render/RenderData.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		class DebugBoxColliderRenderer : public Render::IRenderer, public virtual System::Singleton<DebugBoxColliderRenderer>
		{
			friend class Singleton<DebugBoxColliderRenderer>;
		private:
			Render::Shader shader;
			GLuint vao;
			GLuint vbo;
			std::map<int, std::vector<Render::RenderData*>> renderData;

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