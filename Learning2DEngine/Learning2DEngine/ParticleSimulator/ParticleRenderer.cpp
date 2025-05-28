#include "ParticleRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../Render/RenderManager.h"
#include "../Render/ShaderConstant.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace ParticleSimulator
	{
		ParticleRenderer::ParticleRenderer()
			: BaseMultiRenderer(), particleRenderData()
		{

		}

		void ParticleRenderer::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			shader = resourceManager.IsShaderExist(ShaderConstant::SPRITE_SHADER_NAME)
				? &resourceManager.GetShader(ShaderConstant::SPRITE_SHADER_NAME)
				: &resourceManager.LoadShader(
					ShaderConstant::SPRITE_SHADER_NAME,
					ShaderConstant::GetSpriteVertexShader(),
					ShaderConstant::GetSpriteFragmentShader());
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

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glGenBuffers(1, &vboDynamic);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
			glBufferData(GL_ARRAY_BUFFER, sizeof(MultiSpriteDynamicData), NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)offsetof(MultiSpriteDynamicData, modelMatrix));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)(offsetof(MultiSpriteDynamicData, modelMatrix) + sizeof(float) * 4));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)(offsetof(MultiSpriteDynamicData, modelMatrix) + sizeof(float) * 8));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)(offsetof(MultiSpriteDynamicData, modelMatrix) + sizeof(float) * 12));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)offsetof(MultiSpriteDynamicData, color));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)offsetof(MultiSpriteDynamicData, textureId));

			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			maxObjectSize = 1;
			dynamicData = new MultiSpriteDynamicData[maxObjectSize];
		}

		void ParticleRenderer::DestroyObject()
		{
			BaseMultiRenderer::DestroyObject();

			particleRenderData.clear();
		}

		void ParticleRenderer::SetData(const std::map<int, std::vector<RenderData*>>& renderData)
		{
			GLint maxTextureUnit = RenderManager::GetInstance().GetMaxTextureUnits();
			particleRenderData.clear();

			for (auto& layerData : renderData)
			{
				for (auto& data : layerData.second)
				{
					auto particleData = static_cast<ParticleRenderData*>(data);
					if (!particleData->IsRenderable())
						continue;

					auto particles = particleData->GetParticles();
					size_t activeParticleCount = 0;
					for (size_t i = 0; i < particleData->GetParticleAmount(); ++i)
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

					//If there is minimum 1 active particle, it will be rendered.
					if (activeParticleCount > 0)
					{
						auto& actualLayerData = particleRenderData[layerData.first];

						//Try to find tuple with the same blend function
						//and if the particle has texture, it has to be in the same, which are in the tuple
						//or the texture unit is less than the max texture unit.
						auto it = std::find_if(actualLayerData.begin(),
							actualLayerData.end(),
							[&particleData, maxTextureUnit](auto& data)
							{
								if (std::get<1>(data) != particleData->systemSettings.isUseBlend
									|| std::get<2>(data) != particleData->systemSettings.blendFuncFactor)
									return false;

								if (!particleData->IsUseTexture()
									|| std::get<0>(data).count(particleData->GetTexture()->GetId()) > 0)
									return true;

								return std::get<0>(data).size() < maxTextureUnit;
							});

						//If the layer data is not found, it will be created.
						bool isFound = it != actualLayerData.end();
						if (!isFound)
						{
							actualLayerData.push_back(
								std::make_tuple(
									std::map<GLuint, std::vector<ParticleRenderData*>>(),
									particleData->systemSettings.isUseBlend,
									particleData->systemSettings.blendFuncFactor,
									0)
							);
						}

						auto& actualTuple = !isFound
							? actualLayerData.back()
							: *it;

						GLuint textureId = particleData->IsUseTexture()
							? particleData->GetTexture()->GetId()
							: 0;

						std::get<0>(actualTuple)[textureId].push_back(particleData);

						//Count the active particles by tuple for maxActiveParticleCount
						std::get<3>(actualTuple) += activeParticleCount;
					}
				}
			}

			size_t maxActiveParticleCount = 0;
			for (auto& layerData : particleRenderData)
			{
				for (auto& data : layerData.second)
				{
					if (std::get<3>(data) > maxActiveParticleCount)
						maxActiveParticleCount = std::get<3>(data);
				}
			}

			CalcDynamicDataSize(maxActiveParticleCount);
		}

		void ParticleRenderer::Draw(int layer)
		{
			if (particleRenderData.find(layer) == particleRenderData.end())
				return;

			shader->Use();
			shader->SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader->SetMatrix4("view", Game::mainCamera.GetViewMatrix());
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);

			auto& renderManager = RenderManager::GetInstance();
			for (auto& particleData : particleRenderData[layer])
			{

				//Activate Blend
				BlendFuncFactor previousBlendFuncFactor = renderManager.GetBlendFunc();
				if (std::get<1>(particleData))
				{
					renderManager.SetBlendFunc(std::get<2>(particleData));
				}

				//Collect data
				int activeParticleCount = 0;
				int textureUnitCount = 0;
				for (auto& data : std::get<0>(particleData))
				{
					if (data.first > 0)
					{
						shader->SetInteger(("spriteTextures[" + std::to_string(textureUnitCount) + "]").c_str(), textureUnitCount);
						glActiveTexture(GL_TEXTURE0 + textureUnitCount);
						glBindTexture(GL_TEXTURE_2D, data.first);
						++textureUnitCount; //it can't be bigger than the max texture unit, because we care with it in SetData
					}

					for (auto& particleRenderData : data.second)
					{
						auto particles = particleRenderData->GetParticles();

						for (int i = 0; i < particleRenderData->GetParticleAmount(); ++i)
						{
							if (particles[i].lifeTime > 0.0f)
							{
								std::memcpy(dynamicData[activeParticleCount].modelMatrix,
									glm::value_ptr(particles[i].transform.GetModelMatrix()),
									sizeof(dynamicData[activeParticleCount].modelMatrix));

								std::memcpy(dynamicData[activeParticleCount].color,
									glm::value_ptr(particles[i].color),
									sizeof(dynamicData[activeParticleCount].color));

								dynamicData[activeParticleCount].textureId = particleRenderData->IsUseTexture()
									? textureUnitCount - 1 //because it is incremented before
									: -1.0f;

								++activeParticleCount;
							}
						}
					}

					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(MultiSpriteDynamicData) * activeParticleCount, dynamicData);
					glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, activeParticleCount);

				}

				if (std::get<1>(particleData))
				{
					renderManager.SetBlendFunc(previousBlendFuncFactor);
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}
}