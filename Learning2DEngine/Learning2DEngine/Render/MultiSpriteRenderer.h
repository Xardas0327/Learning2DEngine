#pragma once

#include <vector>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "IRenderer.h"
#include "Shader.h"
#include "SpriteRenderData.h"
#include "MultiSpriteDynamicData.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class MultiSpriteRenderer : public IRenderer, public virtual System::Singleton<MultiSpriteRenderer>
		{
			friend class Singleton<MultiSpriteRenderer>;
		private:
			Shader shader;
			GLuint vao;
			GLuint ebo;
			GLuint vboStatic;
			GLuint vboDynamic;
			unsigned int maxObjectSize;

			//int is the layer
			//GLuint is the texture id. 0 means, that these data doesn't have texture.
			std::map<int, std::map<GLuint, std::vector<SpriteRenderData*>>> spriteRenderData;
			MultiSpriteDynamicData* dynamicData;

			MultiSpriteRenderer();

			void InitShader();
			void InitVao();
			void DestroyObject();
		public:
			void Init() override;
			void Destroy() override;
			//int is the layer
			void SetData(const std::map<int, std::vector<RenderData*>>& renderData) override;
			void Draw(int layer) override;
		};
	}
}