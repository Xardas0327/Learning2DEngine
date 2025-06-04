#include "SimpleText2DRenderer.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../Render/ShaderConstant.h"
#include "../Render/RenderManager.h"
#include "Text2DRenderData.h"
#include "TextCharacterSet.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace UI
	{
		SimpleText2DRenderer::SimpleText2DRenderer()
			: BaseRenderer(), textRenderData()
		{

		}

		void SimpleText2DRenderer::InitShader()
		{
			auto& resourceManager = ResourceManager::GetInstance();

			shader = resourceManager.IsShaderExist(ShaderConstant::SIMPLE_TEXT2D_SHADER_NAME)
				? &resourceManager.GetShader(ShaderConstant::SIMPLE_TEXT2D_SHADER_NAME)
				: &resourceManager.LoadShader(
					ShaderConstant::SIMPLE_TEXT2D_SHADER_NAME,
					ShaderConstant::GetSimpleText2DVertexShader(),
					ShaderConstant::GetSimpleText2DFragmentShader());
		}

		void SimpleText2DRenderer::InitVao()
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

		void SimpleText2DRenderer::DestroyObject()
		{
			BaseRenderer::DestroyObject();

			textRenderData.clear();
		}

		void SimpleText2DRenderer::SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData)
		{
			textRenderData = renderData;
		}

		void SimpleText2DRenderer::Draw(int layer)
		{
			if (textRenderData.find(layer) == textRenderData.end())
				return;

			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			shader->Use();
			shader->SetInteger("characterTexture", 0);
			shader->SetMatrix4("projection", Game::mainCamera.GetProjection());

			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(vao);

			for (auto& data : textRenderData[layer])
			{
				auto textData = static_cast<Text2DRenderData*>(data);

				textCharacterSet.Load(textData->fontSizePair);

				CharacterMap& characterMap = textCharacterSet[textData->fontSizePair];

				shader->SetVector4f("characterColor", textData->color);

				glm::vec2 startPosition(textData->component->gameObject->transform.GetPosition());
				glm::mat2 rotationMatrix = textData->GetRotationMatrix();

				std::string::const_iterator c;
				for (c = textData->GetText().begin(); c != textData->GetText().end(); ++c)
				{
					const auto& ch = characterMap[*c];

					//Calculcate character position
					float chPositionX = ch.bearing.x * textData->component->gameObject->transform.GetScale().x;
					float chPositionY = (characterMap['H'].bearing.y - ch.bearing.y) * textData->component->gameObject->transform.GetScale().y;

					//Calculcate character size
					float chWidth = ch.size.x * textData->component->gameObject->transform.GetScale().x;
					float chHeight = ch.size.y * textData->component->gameObject->transform.GetScale().y;

					glm::vec2 v1 = startPosition + (rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY + chHeight));
					glm::vec2 v2 = startPosition + (rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY));
					glm::vec2 v3 = startPosition + (rotationMatrix * glm::vec2(chPositionX, chPositionY));
					glm::vec2 v4 = startPosition + (rotationMatrix * glm::vec2(chPositionX, chPositionY + chHeight));

					float vertices[4][4] = {
						{ v1.x, v1.y,   1.0f, 1.0f },
						{ v2.x, v2.y,   1.0f, 0.0f },
						{ v3.x, v3.y,   0.0f, 0.0f },
						{ v4.x, v4.y,   0.0f, 1.0f }
					};

					glBindTexture(GL_TEXTURE_2D, ch.textureId);

					glBindBuffer(GL_ARRAY_BUFFER, vbo);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

					// Calculate next character position
					startPosition += rotationMatrix *
						glm::vec2(
							// bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
							(ch.advance >> 6) * textData->component->gameObject->transform.GetScale().x,
							0);
				}
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}