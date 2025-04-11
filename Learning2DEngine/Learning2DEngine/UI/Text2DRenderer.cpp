#include "Text2DRenderer.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../System/ComponentManager.h"
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
		Text2DRenderer::Text2DRenderer()
			: shader(), vao(0), vbo(0), ebo(0), textRenderData()
		{

		}

		void Text2DRenderer::InitShader()
		{
			auto& resourceManager = ResourceManager::GetInstance();

			shader = resourceManager.IsShaderExist(ShaderConstant::TEXT2D_SHADER_NAME)
				? resourceManager.GetShader(ShaderConstant::TEXT2D_SHADER_NAME)
				: resourceManager.LoadShader(
					ShaderConstant::TEXT2D_SHADER_NAME,
					ShaderConstant::GetText2DVertexShader(),
					ShaderConstant::GetText2DFragmentShader());
		}

		void Text2DRenderer::InitVao()
		{
			unsigned int indices[] = {
				0, 1, 3,
				1, 2, 3,
				4, 5, 7,
				5, 6, 7
			};

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 8, NULL, GL_DYNAMIC_DRAW);

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

		void Text2DRenderer::DestroyObject()
		{
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);

			textRenderData.clear();
		}

		void Text2DRenderer::Destroy()
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

		void Text2DRenderer::SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData)
		{
			textRenderData = renderData;
		}

		void Text2DRenderer::Draw(int layer)
		{
			if (textRenderData.find(layer) == textRenderData.end())
				return;

			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			shader.Use();
			shader.SetInteger("characterTextures[0]", 0);
			shader.SetInteger("characterTextures[1]", 1);
			shader.SetMatrix4("projection", Game::mainCamera.GetProjection());

			glBindVertexArray(vao);

			for (auto& data : textRenderData[layer])
			{
				auto textData = static_cast<Text2DRenderData*>(data);

				textCharacterSet.Load(textData->fontSizePair);

				CharacterMap& characterMap = textCharacterSet[textData->fontSizePair];

				shader.SetVector4f("characterColor", textData->color);

				glm::vec2 startPosition(textData->component->gameObject->transform.position);
				glm::mat2 rotationMatrix = textData->GetRotationMatrix();

				std::string::const_iterator c;
				int a = 0;
				for (c = textData->text.begin(); c != textData->text.end(); ++c)
				{
					if (a == 0)
					{
						++a;
						float vertices[32];

						const auto& ch = characterMap[*c];
						float chPositionX = ch.bearing.x * textData->component->gameObject->transform.scale.x;
						float chPositionY = (characterMap['H'].bearing.y - ch.bearing.y) * textData->component->gameObject->transform.scale.y;

						float chWidth = ch.size.x * textData->component->gameObject->transform.scale.x;
						float chHeight = ch.size.y * textData->component->gameObject->transform.scale.y;

						glm::vec2 a = rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY + chHeight);
						glm::vec2 b = rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY);
						glm::vec2 cv = rotationMatrix * glm::vec2(chPositionX, chPositionY);
						glm::vec2 d = rotationMatrix * glm::vec2(chPositionX, chPositionY + chHeight);

						vertices[0] = startPosition.x + a.x;
						vertices[1] = startPosition.y + a.y;
						vertices[2] = 1.0f;
						vertices[3] = 1.0f;
						vertices[4] = startPosition.x + b.x;
						vertices[5] = startPosition.y + b.y;
						vertices[6] = 1.0f;
						vertices[7] = 0.0f;
						vertices[8] = startPosition.x + cv.x;
						vertices[9] = startPosition.y + cv.y;
						vertices[10] = 0.0f;
						vertices[11] = 0.0f;
						vertices[12] = startPosition.x + d.x;
						vertices[13] = startPosition.y + d.y;
						vertices[14] = 0.0f;
						vertices[15] = 1.0f;

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, ch.textureId);

						startPosition += rotationMatrix *
							glm::vec2(
								// bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
								(ch.advance >> 6) * textData->component->gameObject->transform.scale.x,
								0);

						++c;

						const auto& ch2 = characterMap[*c];
						chPositionX = ch2.bearing.x * textData->component->gameObject->transform.scale.x;
						chPositionY = (characterMap['H'].bearing.y - ch2.bearing.y) * textData->component->gameObject->transform.scale.y;

						chWidth = ch2.size.x * textData->component->gameObject->transform.scale.x;
						chHeight = ch2.size.y * textData->component->gameObject->transform.scale.y;

						a = rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY + chHeight);
						b = rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY);
						cv = rotationMatrix * glm::vec2(chPositionX, chPositionY);
						d = rotationMatrix * glm::vec2(chPositionX, chPositionY + chHeight);

						vertices[16] = startPosition.x + a.x;
						vertices[17] = startPosition.y + a.y;
						vertices[18] = 1.0f;
						vertices[19] = 1.0f;
						vertices[20] = startPosition.x + b.x;
						vertices[21] = startPosition.y + b.y;
						vertices[22] = 1.0f;
						vertices[23] = 0.0f;
						vertices[24] = startPosition.x + cv.x;
						vertices[25] = startPosition.y + cv.y;
						vertices[26] = 0.0f;
						vertices[27] = 0.0f;
						vertices[28] = startPosition.x + d.x;
						vertices[29] = startPosition.y + d.y;
						vertices[30] = 0.0f;
						vertices[31] = 1.0f;

						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, ch2.textureId);

						glBindBuffer(GL_ARRAY_BUFFER, vbo);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						glDrawElementsInstanced(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0, 2);

						startPosition += rotationMatrix *
							glm::vec2(
								// bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
								(ch.advance >> 6) * textData->component->gameObject->transform.scale.x,
								0);

						continue;
					}
				}
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}