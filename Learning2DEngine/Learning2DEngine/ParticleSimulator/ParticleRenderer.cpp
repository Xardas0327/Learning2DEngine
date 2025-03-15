#include "ParticleRenderer.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../Render/RenderManager.h"
#include "../Render/ShaderConstant.h"
#include "ParticleRenderData.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace ParticleSimulator
	{
		ParticleRenderer::ParticleRenderer()
			: shader(), vao(0), vbo(0), ebo(0)
		{

		}

		void ParticleRenderer::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			shader = resourceManager.IsShaderExist(ShaderConstant::SPRITE_SHADER_NAME)
				? resourceManager.GetShader(ShaderConstant::SPRITE_SHADER_NAME)
				: resourceManager.LoadShader(
					ShaderConstant::SPRITE_SHADER_NAME,
					ShaderConstant::SPRITE_VERTEX_SHADER,
					ShaderConstant::SPRITE_FRAGMENT_SHADER);
		}

		void ParticleRenderer::InitVao()
		{
			float vertices[] = {
				// pos      // tex
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 1.0f,
			};

			unsigned int indices[] = {
			   0, 1, 3,
			   1, 2, 3
			};

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void ParticleRenderer::Init()
		{
			InitShader();
			InitVao();
		}

		void ParticleRenderer::Destroy()
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
		}

		void ParticleRenderer::Draw(std::vector<RenderData*> renderData)
		{
			shader.Use();
			shader.SetInteger("spriteTexture", 0);
			glBindVertexArray(vao);

			auto& renderManager = RenderManager::GetInstance();
			for (auto data : renderData)
			{
				auto particleData = static_cast<ParticleRenderData*>(data);
				if (!particleData->gameObject->isActive || !particleData->IsRenderable())
					continue;

				BlendFuncFactor previousBlendFuncFactor = renderManager.GetBlendFunc();
				if (particleData->systemSettings.isUseBlend)
				{
					renderManager.SetBlendFunc(particleData->systemSettings.blendFuncFactor);
				}

				if (particleData->IsUseTexture())
				{
					glActiveTexture(GL_TEXTURE0);
					particleData->texture->Bind();
				}
				shader.SetInteger("isUseTexture", particleData->IsUseTexture());

				auto particles = particleData->GetParticles();

				for (int i = 0; i < particleData->GetParticleAmount(); ++i)
				{
					if (particles[i].lifeTime > 0.0f)
					{
						shader.SetMatrix4("model", particles[i].transform.GetModelMatrix());
						shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
						shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());
						shader.SetVector3f("spriteColor", particles[i].color);

						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
					}
				}
				glBindTexture(GL_TEXTURE_2D, 0);


				if (particleData->systemSettings.isUseBlend)
				{
					renderManager.SetBlendFunc(previousBlendFuncFactor);
				}
			}

			glBindVertexArray(0);
		}
	}
}