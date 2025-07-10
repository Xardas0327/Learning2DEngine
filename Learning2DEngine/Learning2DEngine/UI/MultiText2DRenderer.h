#pragma once

#include <map>
#include <vector>
#include <array>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "../Render/BaseMultiRenderer.h"

#include "Text2DDynamicData.h"

namespace Learning2DEngine
{
	namespace UI
	{
		class MultiText2DRenderer : public Render::BaseMultiRenderer<Text2DDynamicData>, public System::Singleton<MultiText2DRenderer>
		{
			friend class Singleton<MultiText2DRenderer>;
		private:
			//The int is the layer  
			//The map key is a GLuint, which a character texture id, and the value is a vector of array,
			//which contains the position, the texture coordinates, the color and isUseCameraView.
			std::map<int, std::map<GLuint, std::vector<std::array<float, 33>>>> textRenderData;

			MultiText2DRenderer();

			void InitShader() override;
			void InitVao() override;
			void DestroyObject() override;
			void CalcDynamicDataSize(size_t maxDynamicSize) override;
		public:
			//int is the layer
			void SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData) override;
			void Draw(int layer) override;
		};
	}
}
