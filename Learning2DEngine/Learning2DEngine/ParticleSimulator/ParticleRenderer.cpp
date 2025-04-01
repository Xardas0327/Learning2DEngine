#include "ParticleRenderer.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../System/ComponentManager.h"
#include "../Render/RenderManager.h"
#include "../Render/ShaderConstant.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace ParticleSimulator
	{
		ParticleRenderer::ParticleRenderer()
			: shader(), vao(0), ebo(0), vboBasic(0), vboModel(0), vboColor(0), maxObjectSize(0),
			particleRenderData(), models(nullptr), colors(nullptr)
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
			glBindVertexArray(vao);

			glGenBuffers(1, &vboBasic);
			glBindBuffer(GL_ARRAY_BUFFER, vboBasic);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glGenBuffers(1, &vboModel);
			glBindBuffer(GL_ARRAY_BUFFER, vboModel);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glGenBuffers(1, &vboColor);
			glBindBuffer(GL_ARRAY_BUFFER, vboColor);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			maxObjectSize = 1;
			models = new glm::mat4[maxObjectSize];
			colors = new glm::vec4[maxObjectSize];
		}

		void ParticleRenderer::Init()
		{
			if (ComponentManager::GetInstance().GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(RenderManager::GetInstance().mutex);
				InitShader();
				InitVao();
			}
			else
			{
				InitShader();
				InitVao();
			}
		}

		void ParticleRenderer::DestroyObject()
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &ebo);
			glDeleteBuffers(1, &vboBasic);
			glDeleteBuffers(1, &vboModel);
			glDeleteBuffers(1, &vboColor);

			particleRenderData.clear();

			//if the model is not null, the colors is also not null
			if (models != nullptr)
			{
				delete[] models;
				delete[] colors;
			}
		}

		void ParticleRenderer::Destroy()
		{
			if (ComponentManager::GetInstance().GetThreadSafe())
			{
				std::lock_guard<std::mutex> lock(RenderManager::GetInstance().mutex);
				DestroyObject();
			}
			else
			{
				DestroyObject();
			}
		}

		void ParticleRenderer::SetData(const std::map<int, std::vector<RenderData*>>& renderData)
		{
			particleRenderData.clear();
			int maxActiveParticleCount = 0;
			for (auto& layerData : renderData)
			{
				for (auto& data : layerData.second)
				{
					auto particleData = static_cast<ParticleRenderData*>(data);
					if (!particleData->IsRenderable())
						continue;

					auto particles = particleData->GetParticles();
					int activeParticleCount = 0;
					for (int i = 0; i < particleData->GetParticleAmount(); ++i)
					{
						if (particles[i].lifeTime > 0.0f)
						{
							++activeParticleCount;
						}
					}

					activeParticleCount =
						particleData->GetMinAllocateSize() > activeParticleCount ?
						particleData->GetMinAllocateSize() :
						activeParticleCount;

					if (activeParticleCount > 0)
					{
						particleRenderData[layerData.first].push_back(particleData);

						if (maxActiveParticleCount < activeParticleCount)
							maxActiveParticleCount = activeParticleCount;
					}
				}
			}

			//if the size is not enough or too big, it will be reallocated.
			if (maxActiveParticleCount > maxObjectSize || maxObjectSize > maxActiveParticleCount * 2)
			{
				//It allocates 20% more space, so that it does not have to allocate again if there are some dynamic renderers. 
				maxObjectSize = static_cast<float>(maxActiveParticleCount) * 1.2f;

				glBindBuffer(GL_ARRAY_BUFFER, vboModel);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * maxObjectSize, NULL, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, vboColor);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * maxObjectSize, NULL, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				delete[] models;
				delete[] colors;
				models = new glm::mat4[maxObjectSize];
				colors = new glm::vec4[maxObjectSize];
			}
		}

		void ParticleRenderer::Draw(int layer)
		{
			if (particleRenderData.find(layer) == particleRenderData.end())
				return;

			shader.Use();
			shader.SetInteger("spriteTexture", 0);
			shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());
			glBindVertexArray(vao);

			auto& renderManager = RenderManager::GetInstance();
			for (auto& particleData : particleRenderData[layer])
			{

				//Activate Blend
				BlendFuncFactor previousBlendFuncFactor = renderManager.GetBlendFunc();
				if (particleData->systemSettings.isUseBlend)
				{
					renderManager.SetBlendFunc(particleData->systemSettings.blendFuncFactor);
				}

				//Activate Texture
				if (particleData->IsUseTexture())
				{
					glActiveTexture(GL_TEXTURE0);
					particleData->texture->Bind();
				}
				shader.SetInteger("isUseTexture", particleData->IsUseTexture());

				//Collect data
				auto particles = particleData->GetParticles();
				int activeParticleCount = 0;
				for (int i = 0; i < particleData->GetParticleAmount(); ++i)
				{
					if (particles[i].lifeTime > 0.0f)
					{
						models[activeParticleCount] = particles[i].transform.GetModelMatrix();
						colors[activeParticleCount] = particles[i].color;
						++activeParticleCount;
					}
				}

				glBindBuffer(GL_ARRAY_BUFFER, vboModel);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * activeParticleCount, &models[0]);
				glBindBuffer(GL_ARRAY_BUFFER, vboColor);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * activeParticleCount, &colors[0]);

				glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, activeParticleCount);

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