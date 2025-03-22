#pragma once

#include <vector>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "../Render/IRenderer.h"
#include "../Render/Shader.h"

namespace Learning2DEngine
{
	namespace UI
	{
		class Text2DRenderer : public Render::IRenderer, public virtual System::Singleton<Text2DRenderer>
		{
			friend class Singleton<Text2DRenderer>;
		private:
			Render::Shader shader;
			GLuint vao;
			GLuint vbo;
			GLuint ebo;

			//int is the layer
			std::map<int, std::vector<Render::RenderData*>> textRenderData;

			Text2DRenderer();

			void InitShader();
			void InitVao();
		public:
			void Init() override;
			void Destroy() override;
			//int is the layer
			void SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData) override;
			void Draw(int layer) override;
		};
	}
}
