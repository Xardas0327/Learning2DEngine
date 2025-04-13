#pragma once

#include <vector>
#include <map>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "../Render/IRenderer.h"
#include "../Render/Shader.h"

namespace Learning2DEngine
{
	namespace UI
	{
		class SimpleText2DRenderer : public Render::IRenderer, public virtual System::Singleton<SimpleText2DRenderer>
		{
			friend class Singleton<SimpleText2DRenderer>;
		private:
			Render::Shader shader;
			GLuint vao;
			GLuint vbo;
			GLuint ebo;

			//int is the layer
			std::map<int, std::vector<Render::RenderData*>> textRenderData;

			SimpleText2DRenderer();

			void InitShader();
			void InitVao();
			void DestroyObject();
		public:
			void Init() override;
			void Destroy() override;
			//int is the layer
			void SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData) override;
			void Draw(int layer) override;
		};
	}
}
