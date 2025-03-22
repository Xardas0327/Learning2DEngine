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
		class SimpleSpriteRenderer : public IRenderer, public virtual System::Singleton<SimpleSpriteRenderer>
		{
			friend class Singleton<SimpleSpriteRenderer>;
		private:
			Shader shader;
			GLuint vao;
			GLuint vbo;
			GLuint ebo;
			std::map<int, std::vector<RenderData*>> spriteRenderData;

			SimpleSpriteRenderer();

			void InitShader();
			void InitVao();
		public:
			void Init() override;
			void Destroy() override;
			void SetData(const std::map<int, std::vector<RenderData*>>& renderData) override;
			void Draw(int layer) override;
		};
	}
}