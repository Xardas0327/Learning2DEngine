#pragma once

#include <glad/glad.h>

#include <vector>
#include <map>

#include "../System/Singleton.h"
#include "IRenderer.h"
#include "Shader.h"
#include "SpriteRenderData.h"
#include "MultiSpriteDynamicData.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class MultiSpriteRenderer : public IRenderer, public System::Singleton<MultiSpriteRenderer>
		{
			friend class Singleton<MultiSpriteRenderer>;
		private:
			Shader* shader;
			GLuint vao;
			GLuint ebo;
			GLuint vboStatic;
			GLuint vboDynamic;
			size_t maxObjectSize;

			//The int is the layer
			//The second map's key is the texture id and the value is the SpriteRenderData vector, which use that texture.
			//If the sprite doesn't use texture, the key is 0.
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