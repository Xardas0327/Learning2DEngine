#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../System/Singleton.h"
#include "IRenderer.h"
#include "Shader.h"
#include "SpriteRenderData.h"

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
			GLuint ebo;
			GLuint vboBasic;
			GLuint vboModel;
			GLuint vboColor;
			unsigned int lastObjectSize;

			//int is the layer
			//GLuint is the texture id.
			//0 means, that these data doesn't have texture.
			std::map<int, std::map<GLuint, std::vector<SpriteRenderData*>>> spriteRenderData;
			glm::mat4* models;
			glm::vec4* colors;

			SpriteRenderer();

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