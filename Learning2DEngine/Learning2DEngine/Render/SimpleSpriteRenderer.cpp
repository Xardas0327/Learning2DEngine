#include "SimpleSpriteRenderer.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "ShaderConstant.h"
#include "SpriteRenderData.h"
#include "RenderManager.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{

		SimpleSpriteRenderer::SimpleSpriteRenderer()
			: BaseRenderer(), spriteRenderData()
		{

		}

		void SimpleSpriteRenderer::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			shader = resourceManager.IsShaderExist(ShaderConstant::SIMPLE_SPRITE_SHADER_NAME)
				? &resourceManager.GetShader(ShaderConstant::SIMPLE_SPRITE_SHADER_NAME)
				: &resourceManager.LoadShader(
					ShaderConstant::SIMPLE_SPRITE_SHADER_NAME,
					ShaderConstant::GetSimpleSpriteVertexShader(),
					ShaderConstant::GetSimpleSpriteFragmentShader());
		}

		void SimpleSpriteRenderer::InitVao()
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

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void SimpleSpriteRenderer::DestroyObject()
		{
			BaseRenderer::DestroyObject();

			spriteRenderData.clear();
		}

		void SimpleSpriteRenderer::SetData(const std::map<int, std::vector<RenderData*>>& renderData)
		{
			spriteRenderData = renderData;
		}

		void SimpleSpriteRenderer::Draw(int layer)
		{
			if (spriteRenderData.find(layer) == spriteRenderData.end())
				return;

			shader->Use();
			shader->SetInteger("spriteTexture", 0);
			shader->SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader->SetMatrix4("view", Game::mainCamera.GetViewMatrix());
			glBindVertexArray(vao);

			for (auto data : spriteRenderData[layer])
			{
				auto spriteData = static_cast<SpriteRenderData*>(data);

				shader->SetMatrix4("model", spriteData->component->gameObject->transform.GetModelMatrix());
				shader->SetInteger("useView", spriteData->isUseCameraView ? GL_TRUE : GL_FALSE);
				shader->SetVector4f("spriteColor", spriteData->color);
				shader->SetInteger("isUseTexture", spriteData->IsUseTexture());

				if (spriteData->IsUseTexture())
				{
					glActiveTexture(GL_TEXTURE0);
					spriteData->GetTexture()->Bind();
				}

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			glBindVertexArray(0);
		}
	}
}