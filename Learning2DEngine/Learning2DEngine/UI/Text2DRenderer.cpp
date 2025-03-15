#include "Text2DRenderer.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../Render/ShaderConstant.h"
#include "Text2DRenderData.h"
#include "TextCharacterSet.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace UI
	{
		Text2DRenderer::Text2DRenderer()
			: shader(), vao(0), vbo(0), ebo(0)
		{

		}

		void Text2DRenderer::InitShader()
		{
			auto& resourceManager = ResourceManager::GetInstance();

			shader = resourceManager.IsShaderExist(ShaderConstant::TEXT2D_SHADER_NAME)
				? resourceManager.GetShader(ShaderConstant::TEXT2D_SHADER_NAME)
				: resourceManager.LoadShader(
					ShaderConstant::TEXT2D_SHADER_NAME,
					ShaderConstant::TEXT2D_VERTEX_SHADER,
					ShaderConstant::TEXT2D_FRAGMENT_SHADER);
		}

		void Text2DRenderer::InitVao()
		{
			unsigned int indices[] = {
				0, 1, 3,
				1, 2, 3
			};

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void Text2DRenderer::Init()
		{
			InitShader();
			InitVao();
		}
		void Text2DRenderer::Destroy()
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
		}

		void Text2DRenderer::Draw(std::vector<RenderData*> renderData)
		{
			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			shader.Use();
			shader.SetInteger("characterTexture", 0);
			shader.SetMatrix4("projection", Game::mainCamera.GetProjection());

			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(vao);

			for (auto data : renderData)
			{
				auto textData = static_cast<Text2DRenderData*>(data);

				textCharacterSet.Load(textData->fontSizePair);

				CharacterMap& characterMap = textCharacterSet[textData->fontSizePair];

				shader.SetVector4f("characterColor", textData->color);

				glm::vec2 startPosition(textData->component->gameObject->transform.position);
				glm::mat2 rotationMatrix = textData->GetRotationMatrix();

				std::string::const_iterator c;
				for (c = textData->text.begin(); c != textData->text.end(); ++c)
				{
					const auto& ch = characterMap[*c];

					float chPositionX = ch.bearing.x * textData->component->gameObject->transform.scale.x;
					float chPositionY = (characterMap['H'].bearing.y - ch.bearing.y) * textData->component->gameObject->transform.scale.y;

					float chWidth = ch.size.x * textData->component->gameObject->transform.scale.x;
					float chHeight = ch.size.y * textData->component->gameObject->transform.scale.y;

					glm::vec2 a = rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY + chHeight);
					glm::vec2 b = rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY);
					glm::vec2 c = rotationMatrix * glm::vec2(chPositionX, chPositionY);
					glm::vec2 d = rotationMatrix * glm::vec2(chPositionX, chPositionY + chHeight);

					float vertices[4][4] = {
						{ startPosition.x + a.x, startPosition.y + a.y,   1.0f, 1.0f },
						{ startPosition.x + b.x, startPosition.y + b.y,   1.0f, 0.0f },
						{ startPosition.x + c.x, startPosition.y + c.y,   0.0f, 0.0f },
						{ startPosition.x + d.x, startPosition.y + d.y,   0.0f, 1.0f }
					};

					glBindTexture(GL_TEXTURE_2D, ch.textureId);

					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

					startPosition += rotationMatrix *
						glm::vec2(
							// bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
							(ch.advance >> 6) * textData->component->gameObject->transform.scale.x,
							0);
				}
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}