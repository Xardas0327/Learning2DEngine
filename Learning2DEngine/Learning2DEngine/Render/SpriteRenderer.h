#pragma once

#include <vector>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "IRenderer.h"
#include "Shader.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class SpriteRenderer : public IRenderer, public virtual System::Singleton<SpriteRenderer>
		{
			friend class Singleton<SpriteRenderer>;
		private:
			Shader shader;
			GLuint vao;
			GLuint vbo;
			GLuint ebo;

			SpriteRenderer();

			void InitShader();
			void InitVao();
		public:
			void Init() override;
			void Destroy() override;
			void Draw(std::vector<RenderData*> renderData) override;
		};
	}
}