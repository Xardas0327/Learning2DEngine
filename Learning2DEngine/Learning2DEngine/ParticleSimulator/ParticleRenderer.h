#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../System/Singleton.h"
#include "../Render/IRenderer.h"
#include "../Render/Shader.h"

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
			GLuint vboBasic;
			GLuint vboModel;
			GLuint vboColor;
			unsigned int maxObjectSize;

			//int is the layer
			std::map<int, std::vector<ParticleRenderData*>> particleRenderData;
			glm::mat4* models;
			glm::vec4* colors;

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