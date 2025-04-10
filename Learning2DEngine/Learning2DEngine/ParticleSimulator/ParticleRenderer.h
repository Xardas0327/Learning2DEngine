#pragma once

#include <vector>

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
			//The tuple contains the vector of SpriteRenderData* and a map of textures.
			//GLuint is the texture id and the int is the texture unit id.
			//bool means, that it use blend or not.
			//BlendFuncFactor contains the blend function factor.
			//Last int is the max active particle count.
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