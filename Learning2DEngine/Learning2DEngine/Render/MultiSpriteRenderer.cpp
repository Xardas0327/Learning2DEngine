#include "MultiSpriteRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "ShaderConstant.h"
#include "RenderManager.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{

		MultiSpriteRenderer::MultiSpriteRenderer()
			: BaseMultiRenderer(), spriteRenderData()
		{

		}

		void MultiSpriteRenderer::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			shader = resourceManager.IsShaderExist(ShaderConstant::SPRITE_SHADER_NAME)
				? &resourceManager.GetShader(ShaderConstant::SPRITE_SHADER_NAME)
				: &resourceManager.LoadShader(
					ShaderConstant::SPRITE_SHADER_NAME,
					ShaderConstant::GetSpriteVertexShader(),
					ShaderConstant::GetSpriteFragmentShader());
		}

		void MultiSpriteRenderer::InitVao()
		{
			float positions[] = {
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f,
			};

			unsigned int indices[] = {
			   0, 1, 3,
			   1, 2, 3
			};

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

			glGenBuffers(1, &vboDynamicUV);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamicUV);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat2x4), NULL, GL_DYNAMIC_DRAW);

			for (int i = 0; i < 4; ++i) {
				glEnableVertexAttribArray(1 + i);
				glVertexAttribPointer(1 + i, 2, GL_FLOAT, GL_FALSE, sizeof(glm::mat2x4), (void*)(sizeof(float) * 2 * i));
				glVertexAttribDivisor(1 + i, 1);
			}

			glGenBuffers(1, &vboDynamicObject);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamicObject);
			glBufferData(GL_ARRAY_BUFFER, sizeof(MultiSpriteDynamicData), NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)offsetof(MultiSpriteDynamicData, modelMatrix));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)(offsetof(MultiSpriteDynamicData, modelMatrix) + sizeof(float) * 4));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)(offsetof(MultiSpriteDynamicData, modelMatrix) + sizeof(float) * 8));
			glEnableVertexAttribArray(8);
			glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)(offsetof(MultiSpriteDynamicData, modelMatrix) + sizeof(float) * 12));
			glEnableVertexAttribArray(9);
			glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)offsetof(MultiSpriteDynamicData, color));
			glEnableVertexAttribArray(10);
			glVertexAttribPointer(10, 1, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)offsetof(MultiSpriteDynamicData, textureId));
			glEnableVertexAttribArray(11);
			glVertexAttribPointer(11, 1, GL_FLOAT, GL_FALSE,
				sizeof(MultiSpriteDynamicData),
				(void*)offsetof(MultiSpriteDynamicData, isUseCameraView));

			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);
			glVertexAttribDivisor(9, 1);
			glVertexAttribDivisor(10, 1);
			glVertexAttribDivisor(11, 1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			maxObjectSize = 1;
			dynamicData = new MultiSpriteDynamicData[maxObjectSize];
		}

		void MultiSpriteRenderer::DestroyObject()
		{
			BaseMultiRenderer::DestroyObject();

			spriteRenderData.clear();
		}

		void MultiSpriteRenderer::SetData(const std::map<RendererMode, std::map<int, std::vector<RenderData*>>>& renderData)
		{
			GLint maxTextureUnit = RenderManager::GetInstance().GetMaxTextureUnits();
			spriteRenderData.clear();
			size_t maxDynamicSize = 0;
			for(auto& modeData : renderData)
			{
				size_t actualDynamicSize = 0;
				for (auto& layerData : modeData.second)
				{
					auto& actualLayerData = spriteRenderData[modeData.first][layerData.first];
					for (auto& data : layerData.second)
					{
						auto spriteData = static_cast<SpriteRenderData*>(data);

						if (spriteData->IsUseTexture())
							actualLayerData[spriteData->GetTexture()->GetId()].push_back(spriteData);
						else
							//The 0 is invalid texture id.
							actualLayerData[0].push_back(spriteData);
					}

					int textureUnitCount = 0;
					int itemCount = 0;
					size_t actualMaxSize = 0;

					for (auto& data : spriteRenderData[modeData.first][layerData.first])
					{
						actualMaxSize += data.second.size();

						// Don't count the invalid texture id.
						if (data.first > 0)
							++textureUnitCount;

						++itemCount;
						// Check if the texture unit number is arrived to max or this is the last data
						if (textureUnitCount >= maxTextureUnit || itemCount == spriteRenderData[modeData.first][layerData.first].size())
						{
							if (actualMaxSize > actualDynamicSize)
								actualDynamicSize = actualMaxSize;

							actualMaxSize = 0;
							textureUnitCount = 0;
						}
					}
				}

				if(maxDynamicSize < actualDynamicSize)
					maxDynamicSize = actualDynamicSize;
			}

			CalcDynamicDataSize(maxDynamicSize);
		}

		void MultiSpriteRenderer::Draw(RendererMode rendererMode, int layer)
		{
			if (spriteRenderData.find(rendererMode) == spriteRenderData.end())
				return;
			if (spriteRenderData[rendererMode].find(layer) == spriteRenderData[rendererMode].end())
				return;

			GLint maxTextureUnit = RenderManager::GetInstance().GetMaxTextureUnits();

			shader->Use();
			shader->SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader->SetMatrix4("view", Game::mainCamera.GetViewMatrix());
			glBindVertexArray(vao);

			int textureUnitCount = 0;
			int dataCount = 0;
			int dynamicDataCount = 0;
			for (auto& data : spriteRenderData[rendererMode][layer])
			{
				if (data.first > 0)
				{
					shader->SetInteger(("spriteTextures[" + std::to_string(textureUnitCount) + "]").c_str(), textureUnitCount);
					glActiveTexture(GL_TEXTURE0 + textureUnitCount);
					glBindTexture(GL_TEXTURE_2D, data.first);
					++textureUnitCount;
				}

				for (size_t i = 0; i < data.second.size(); ++i)
				{
					std::memcpy(dynamicData[dynamicDataCount].modelMatrix,
						glm::value_ptr(data.second[i]->component->gameObject->transform.GetModelMatrix()),
						sizeof(dynamicData[dynamicDataCount].modelMatrix));

					std::memcpy(dynamicData[dynamicDataCount].color,
						glm::value_ptr(data.second[i]->color),
						sizeof(dynamicData[dynamicDataCount].color));

					dynamicData[dynamicDataCount].textureId = data.second[i]->IsUseTexture()
						? textureUnitCount - 1 //because it is incremented before
						: -1.0f;

					dynamicData[dynamicDataCount].isUseCameraView = static_cast<float>(data.second[i]->isUseCameraView);

					++dynamicDataCount;
				}
				++dataCount;

				// Check if the texture unit number is arrived to max or this is the last data
				if (textureUnitCount >= maxTextureUnit || dataCount == spriteRenderData[rendererMode][layer].size())
				{
					glBindBuffer(GL_ARRAY_BUFFER, vboDynamicUV);
					glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat2x4) * dynamicDataCount, NULL, GL_DYNAMIC_DRAW);

					glm::mat2x4* a = new glm::mat2x4[dynamicDataCount];
					for (size_t i = 0; i < dynamicDataCount; i++)
					{
						a[i] = L2DE_SPRITE_UV_DEFAULT;
					}
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat2x4) * dynamicDataCount, a);

					glBindBuffer(GL_ARRAY_BUFFER, vboDynamicObject);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(MultiSpriteDynamicData) * dynamicDataCount, dynamicData);
					glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, dynamicDataCount);

					dynamicDataCount = 0;
					textureUnitCount = 0;
					delete[] a;
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}
}