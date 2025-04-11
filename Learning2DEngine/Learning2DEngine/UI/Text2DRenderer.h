#pragma once

#include <map>
#include <vector>
#include <tuple>
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
		class Text2DRenderer : public Render::IRenderer, public virtual System::Singleton<Text2DRenderer>
		{
			friend class Singleton<Text2DRenderer>;
		private:
			Render::Shader shader;
			GLuint vao;
			GLuint ebo;
			GLuint vboStatic;
			GLuint vboDynamic;
			GLuint vboTemp;
			unsigned int maxObjectSize;

			//int is the layer
			//The vector contains maps, which renderable at the same time.  
			//The map key is a GLuint, which a character texture id, and the value is a vector of tuples, which contains the vertex position and color.
			std::map<int, std::vector<std::map<GLuint, std::vector<std::tuple<std::array<float, 8>, std::array<float, 4>>>>>> textRenderData;
			Text2DDynamicData* dynamicData;

			Text2DRenderer();

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
