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
			: shader(), vao(0), ebo(0), vboStatic(0), vboDynamic(0), maxObjectSize(0),
			spriteRenderData(), dynamicData(nullptr)
		{

		}

		void MultiSpriteRenderer::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			shader = resourceManager.IsShaderExist(ShaderConstant::SPRITE_SHADER_NAME)
				? resourceManager.GetShader(ShaderConstant::SPRITE_SHADER_NAME)
				: resourceManager.LoadShader(
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
			int maxDynamicSize = 0;
			for (auto& layerData : renderData)
			{
				auto& actualLayerData = spriteRenderData[layerData.first];
				actualLayerData.push_back(
					std::make_tuple(std::vector<SpriteRenderData*>(), std::map<GLuint, int>())
				);

				for (auto& data : layerData.second)
				{
					auto spriteData = static_cast<SpriteRenderData*>(data);

					if (spriteData->IsUseTexture())
					{
						bool isFound = false;
						for (int i = 0; i < actualLayerData.size(); ++i)
						{
							if (std::get<1>(actualLayerData[i]).count(spriteData->texture->GetId()) > 0)
							{
								std::get<0>(actualLayerData[i]).push_back(spriteData);
								isFound = true;
								break;
							}

							if (std::get<1>(actualLayerData[i]).size() < maxTextureUnit)
							{
								std::get<1>(actualLayerData[i])[spriteData->texture->GetId()] = std::get<1>(actualLayerData[i]).size();
								std::get<0>(actualLayerData[i]).push_back(spriteData);
								isFound = true;
								break;
							}
						}

						if (!isFound)
						{
							actualLayerData.push_back(
								std::make_tuple(std::vector<SpriteRenderData*>(), std::map<GLuint, int>())
							);
							std::get<1>(actualLayerData.back())[spriteData->texture->GetId()] = 0;
							std::get<0>(actualLayerData.back()).push_back(spriteData);
						}
					}
					else
					{
						std::get<0>(actualLayerData[0]).push_back(spriteData);
					}
				}

				for (auto& data : spriteRenderData[layerData.first])
				{
					if (std::get<0>(data).size() > maxDynamicSize)
						maxDynamicSize = std::get<0>(data).size();
				}
			}

			//if the size is not enough or too big, it will be reallocated.
			if (maxDynamicSize > maxObjectSize || maxObjectSize > maxDynamicSize * 2)
			{
				//It allocates 20% more space, so that it does not have to allocate again
				//if there are some dynamic renderers. 
				maxObjectSize = static_cast<float>(maxDynamicSize) * 1.2f;

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

			shader.Use();
			shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());
			glBindVertexArray(vao);

			for (auto& data : spriteRenderData[layer])
			{
				for (auto& textureIds : std::get<1>(data))
				{
					glActiveTexture(GL_TEXTURE0 + textureIds.second);
					glBindTexture(GL_TEXTURE_2D, textureIds.first);
					std::string shaderId = "spriteTextures[" + std::to_string(textureIds.second) + "]";
					shader.SetInteger(shaderId.c_str(), textureIds.second);
				}

				auto& sprites = std::get<0>(data);
				for (size_t i = 0; i < sprites.size(); ++i)
				{
					std::memcpy(dynamicData[i].modelMatrix,
						glm::value_ptr(sprites[i]->component->gameObject->transform.GetModelMatrix()),
						sizeof(dynamicData[i].modelMatrix));

					std::memcpy(dynamicData[i].color,
						glm::value_ptr(sprites[i]->color),
						sizeof(dynamicData[i].color));

					dynamicData[i].textureId = sprites[i]->IsUseTexture()
						? std::get<1>(data)[sprites[i]->texture->GetId()]
						: -1.0f;
				}

				glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(MultiSpriteDynamicData) * sprites.size(), dynamicData);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, sprites.size());
			}

			glBindVertexArray(0);
		}
	}
}