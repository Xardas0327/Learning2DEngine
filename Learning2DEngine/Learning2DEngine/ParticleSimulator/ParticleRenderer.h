#pragma once

#include <vector>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "../Render/IRenderer.h"
#include "../Render/Shader.h"


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
			GLuint vbo;
			GLuint ebo;

			ParticleRenderer();

			void InitShader();
			void InitVao();
		public:
			void Init() override;
			void Destroy() override;
			void Draw(const std::vector<Render::RenderData*>& renderData) override;

		};
	}
}