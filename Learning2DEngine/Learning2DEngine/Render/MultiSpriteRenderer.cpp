#include "MultiSpriteRenderer.h"

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
			: shader(), vao(0), ebo(0), vboBasic(0), vboModel(0), vboColor(0), maxObjectSize(0),
			spriteRenderData(), models(nullptr), colors(nullptr)
		{

		}

		void MultiSpriteRenderer::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			shader = resourceManager.IsShaderExist(ShaderConstant::SPRITE_SHADER_NAME)
				? resourceManager.GetShader(ShaderConstant::SPRITE_SHADER_NAME)
				: resourceManager.LoadShader(
					ShaderConstant::SPRITE_SHADER_NAME,
					ShaderConstant::SPRITE_VERTEX_SHADER,
					ShaderConstant::SPRITE_FRAGMENT_SHADER);
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

			glGenBuffers(1, &vboBasic);
			glBindBuffer(GL_ARRAY_BUFFER, vboBasic);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

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
			glDeleteBuffers(1, &vboBasic);
			glDeleteBuffers(1, &vboModel);
			glDeleteBuffers(1, &vboColor);

			spriteRenderData.clear();

			//if the model is not null, the colors is also not null
			if (models != nullptr)
			{
				delete[] models;
				delete[] colors;
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
			spriteRenderData.clear();
			int maxSize = 0;
			for (auto& layerData : renderData)
			{
				for (auto& data : layerData.second)
				{
					auto spriteData = static_cast<SpriteRenderData*>(data);
					if (spriteData->IsUseTexture())
						spriteRenderData[layerData.first][spriteData->texture->GetId()].push_back(spriteData);
					else
						spriteRenderData[layerData.first][0].push_back(spriteData);
				}

				for (auto& data : spriteRenderData[layerData.first])
				{
					if (data.second.size() > maxSize)
						maxSize = data.second.size();
				}
			}

			//if the size is not enough or too big, it will be reallocated.
			if (maxSize > maxObjectSize || maxObjectSize > maxSize * 2)
			{
				glBindBuffer(GL_ARRAY_BUFFER, vboModel);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * maxSize, NULL, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, vboColor);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * maxSize, NULL, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				maxObjectSize = maxSize;

				delete[] models;
				delete[] colors;
				models = new glm::mat4[maxObjectSize];
				colors = new glm::vec4[maxObjectSize];
			}
		}

		void MultiSpriteRenderer::Draw(int layer)
		{
			if (spriteRenderData.find(layer) == spriteRenderData.end())
				return;

			shader.Use();
			shader.SetInteger("spriteTexture", 0);
			shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());
			glBindVertexArray(vao);

			for (auto& data : spriteRenderData[layer])
			{
				if (data.first != 0)
				{
					glActiveTexture(GL_TEXTURE0);
					data.second[0]->texture->Bind();
				}
				shader.SetInteger("isUseTexture", data.first != 0);

				for (size_t i = 0; i < data.second.size(); ++i)
				{
					models[i] = data.second[i]->component->gameObject->transform.GetModelMatrix();
					colors[i] = data.second[i]->color;
				}

				glBindBuffer(GL_ARRAY_BUFFER, vboModel);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * data.second.size(), &models[0]);
				glBindBuffer(GL_ARRAY_BUFFER, vboColor);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * data.second.size(), &colors[0]);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, data.second.size());


				glBindTexture(GL_TEXTURE_2D, 0);
			}

			glBindVertexArray(0);
		}
	}
}