#include "MultiSpriteRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../System/ComponentManager.h"
#include "ShaderConstant.h"
#include "RenderManager.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{

		MultiSpriteRenderer::MultiSpriteRenderer()
			: shader(nullptr), vao(0), ebo(0), vboStatic(0), vboDynamic(0), maxObjectSize(0),
			spriteRenderData(), dynamicData(nullptr)
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

			glGenBuffers(1, &vboStatic);
			glBindBuffer(GL_ARRAY_BUFFER, vboStatic);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

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

		void MultiSpriteRenderer::Init()
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

		void MultiSpriteRenderer::DestroyObject()
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &ebo);
			glDeleteBuffers(1, &vboStatic);
			glDeleteBuffers(1, &vboDynamic);

			spriteRenderData.clear();

			if (dynamicData != nullptr)
			{
				delete[] dynamicData;
			}
		}

		void MultiSpriteRenderer::Destroy()
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

		void MultiSpriteRenderer::SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData)
		{
			GLint maxTextureUnit = RenderManager::GetInstance().GetMaxTextureUnits();
			spriteRenderData.clear();
			size_t maxDynamicSize = 0;
			for (auto& layerData : renderData)
			{
				auto& actualLayerData = spriteRenderData[layerData.first];

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

				for (auto& data : spriteRenderData[layerData.first])
				{
					actualMaxSize += data.second.size();

					// Don't count the invalid texture id.
					if (data.first > 0)
						++textureUnitCount;

					++itemCount;
					// Check if the texture unit number is arrived to max or this is the last data
					if (textureUnitCount >= maxTextureUnit || itemCount == spriteRenderData[layerData.first].size())
					{
						if (actualMaxSize > maxDynamicSize)
							maxDynamicSize = actualMaxSize;

						actualMaxSize = 0;
						textureUnitCount = 0;
					}
				}
			}

			//if the size is not enough or too big, it will be reallocated.
			if (maxDynamicSize > maxObjectSize || maxObjectSize > maxDynamicSize * 2)
			{
				//It allocates 20% more space, so that it does not have to allocate again
				//if there are some dynamic renderers. 
				maxObjectSize = static_cast<size_t>(
					static_cast<float>(maxDynamicSize) * 1.2f
				);

				glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
				glBufferData(GL_ARRAY_BUFFER, sizeof(MultiSpriteDynamicData) * maxObjectSize, NULL, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				delete[] dynamicData;
				dynamicData = new MultiSpriteDynamicData[maxObjectSize];
			}
		}

		void MultiSpriteRenderer::Draw(int layer)
		{
			if (spriteRenderData.find(layer) == spriteRenderData.end())
				return;

			GLint maxTextureUnit = RenderManager::GetInstance().GetMaxTextureUnits();

			shader->Use();
			shader->SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader->SetMatrix4("view", Game::mainCamera.GetViewMatrix());
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);

			int textureUnitCount = 0;
			int dataCount = 0;
			int dynamicDataCount = 0;
			for (auto& data : spriteRenderData[layer])
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

					++dynamicDataCount;
				}
				++dataCount;

				// Check if the texture unit number is arrived to max or this is the last data
				if (textureUnitCount >= maxTextureUnit || dataCount == spriteRenderData[layer].size())
				{
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(MultiSpriteDynamicData) * dynamicDataCount, dynamicData);
					glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, dynamicDataCount);

					dynamicDataCount = 0;
					textureUnitCount = 0;
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}
}