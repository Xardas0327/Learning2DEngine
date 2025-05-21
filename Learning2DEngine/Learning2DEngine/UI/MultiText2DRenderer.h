#pragma once

#include <map>
#include <vector>
#include <array>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "../Render/IRenderer.h"
#include "../Render/Shader.h"

#include "Text2DDynamicData.h"

namespace Learning2DEngine
{
	namespace UI
	{
		class MultiText2DRenderer : public Render::IRenderer, public System::Singleton<MultiText2DRenderer>
		{
			friend class Singleton<MultiText2DRenderer>;
		private:
			Render::Shader shader;
			GLuint vao;
			GLuint ebo;
			GLuint vboDynamic;
			unsigned int maxObjectSize;

			//The int is the layer  
			//The map key is a GLuint, which a character texture id, and the value is a vector of array,
			//which contains the position, the texture coordinates and the color.
			std::map<int, std::map<GLuint, std::vector<std::array<float, 32>>>> textRenderData;
			Text2DDynamicData* dynamicData;

			MultiText2DRenderer();

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
