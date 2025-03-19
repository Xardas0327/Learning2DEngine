#include "SpriteRenderer.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "ShaderConstant.h"
#include "SpriteRenderData.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{

		SpriteRenderer::SpriteRenderer()
			: shader(), vao(0), vbo(0), ebo(0)
		{

		}

		void SpriteRenderer::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			shader = resourceManager.IsShaderExist(ShaderConstant::OLD_SPRITE_SHADER_NAME)
				? resourceManager.GetShader(ShaderConstant::OLD_SPRITE_SHADER_NAME)
				: resourceManager.LoadShader(
					ShaderConstant::OLD_SPRITE_SHADER_NAME,
					ShaderConstant::OLD_SPRITE_VERTEX_SHADER,
					ShaderConstant::OLD_SPRITE_FRAGMENT_SHADER);
		}

		void SpriteRenderer::InitVao()
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

		void SpriteRenderer::Init()
		{
			InitShader();
			InitVao();
		}

		void SpriteRenderer::Destroy()
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
		}

		void SpriteRenderer::Draw(std::vector<RenderData*> renderData)
		{
			shader.Use();
			shader.SetInteger("spriteTexture", 0);
			glBindVertexArray(vao);

			for (auto data : renderData)
			{
				auto spriteData = static_cast<SpriteRenderData*>(data);

				shader.SetMatrix4("model", spriteData->component->gameObject->transform.GetModelMatrix());
				shader.SetMatrix4("projection", Game::mainCamera.GetProjection());
				shader.SetMatrix4("view", Game::mainCamera.GetViewMatrix());

				shader.SetVector4f("spriteColor", spriteData->color);
				shader.SetInteger("isUseTexture", spriteData->IsUseTexture());

				if (spriteData->IsUseTexture())
				{
					glActiveTexture(GL_TEXTURE0);
					spriteData->texture->Bind();
				}

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}


			glBindVertexArray(0);
		}
	}
}