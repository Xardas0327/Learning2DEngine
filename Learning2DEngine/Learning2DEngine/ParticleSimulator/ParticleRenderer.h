#pragma once

#include <map>
#include <vector>
#include <tuple>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "../Render/IRenderer.h"
#include "../Render/Shader.h"
#include "../Render/MultiSpriteDynamicData.h"

#include "ParticleRenderData.h"


namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		class ParticleRenderer : public Render::IRenderer, public virtual System::Singleton<ParticleRenderer>
		{
			friend class Singleton<ParticleRenderer>;
		private:
			Render::Shader shader;
			GLuint vao;
			GLuint ebo;
			GLuint vboStatic;
			GLuint vboDynamic;
			unsigned int maxObjectSize;

			//The first int is the layer
			//The vector contains the tuples, which renderable at the same time.  
			//The tuple contains the vector of SpriteRenderData*, the texture ids with texture unit ids,
			//a bool which means, that it uses blend or not, the blend function factor and the max active particle count.
			std::map<int, std::vector<std::tuple<std::vector<ParticleRenderData*>, std::map<GLuint, int>, bool, Render::BlendFuncFactor, int>>> particleRenderData;
			Render::MultiSpriteDynamicData* dynamicData;

			ParticleRenderer();

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