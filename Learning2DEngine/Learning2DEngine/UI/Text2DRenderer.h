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

			Text2DRenderer();

			void InitShader();
			void InitVao();
		public:
			void Init() override;
			void Destroy() override;
			void Draw(const std::vector<Render::RenderData*>& renderData) override;
		};
	}
}
