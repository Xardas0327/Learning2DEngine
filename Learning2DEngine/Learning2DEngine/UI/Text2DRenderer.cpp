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
			: shader(), vao(0), ebo(0), vboStatic(0), vboDynamic(0),
			textRenderData(), dynamicData(nullptr)
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
			float textureCoordinate[] = {
				1.0f, 1.0f,
				1.0f, 0.0f,
				0.0f, 0.0f,
				0.0f, 1.0f,
			};

			unsigned int indices[] = {
				0, 1, 3,
				1, 2, 3
			};

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vboStatic);
			glBindBuffer(GL_ARRAY_BUFFER, vboStatic);
			glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinate), textureCoordinate, GL_STATIC_DRAW);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

			glGenBuffers(1, &vboDynamic);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 4, NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

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
			glDeleteBuffers(1, &ebo);
			glDeleteBuffers(1, &vboStatic);
			glDeleteBuffers(1, &vboDynamic);

			textRenderData.clear();

			if (dynamicData != nullptr)
			{
				delete[] dynamicData;
			}
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
			GLint maxTextureUnit = RenderManager::GetInstance().GetMaxTextureUnits();
			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			textRenderData.clear();
			for (auto& layerData : renderData)
			{
				auto& actualLayerData = textRenderData[layerData.first];

				for (auto& data : layerData.second)
				{
					auto textData = static_cast<Text2DRenderData*>(data);
					if (textData->text.size() > 0)
					{
						textCharacterSet.Load(textData->fontSizePair);
						CharacterMap& characterMap = textCharacterSet[textData->fontSizePair];

						glm::vec2 startPosition(textData->component->gameObject->transform.position);
						glm::mat2 rotationMatrix = textData->GetRotationMatrix();

						for (std::string::const_iterator c = textData->text.begin(); c != textData->text.end(); ++c)
						{
							const auto& ch = characterMap[*c];

							//Try to find map which contains the character
							//or the character number is less than the max texture unit.
							auto it = std::find_if(actualLayerData.begin(),
								actualLayerData.end(),
								[maxTextureUnit, &ch](auto& map)
								{
									return map.count(ch.textureId) > 0 || map.size() < maxTextureUnit;
								});

							//If the layer data is not found, it will be created.
							bool isFound = it != actualLayerData.end();
							if (!isFound)
							{
								actualLayerData.emplace_back();
							}

							auto& actualMap = !isFound
								? actualLayerData.back()
								: *it;

							//Calculcate character position
							float chPositionX = ch.bearing.x * textData->component->gameObject->transform.scale.x;
							float chPositionY = (characterMap['H'].bearing.y - ch.bearing.y) * textData->component->gameObject->transform.scale.y;

							//Calculcate character size
							float chWidth = ch.size.x * textData->component->gameObject->transform.scale.x;
							float chHeight = ch.size.y * textData->component->gameObject->transform.scale.y;

							glm::vec2 point1 = startPosition + (rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY + chHeight)) ;
							glm::vec2 point2 = startPosition + (rotationMatrix * glm::vec2(chPositionX + chWidth, chPositionY));
							glm::vec2 point3 = startPosition + (rotationMatrix * glm::vec2(chPositionX, chPositionY));
							glm::vec2 point4 = startPosition + (rotationMatrix * glm::vec2(chPositionX, chPositionY + chHeight));

							actualMap[ch.textureId].emplace_back(
								std::make_tuple(
									std::array<float, 8>{ point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, point4.x, point4.y },
									std::array<float, 4>{ textData->color.r, textData->color.g, textData->color.b, textData->color.a }
								)
							);

							//Calculate next character postion
							startPosition += rotationMatrix *
								glm::vec2(
									// bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
									(ch.advance >> 6) * textData->component->gameObject->transform.scale.x,
									0);
						}
					}

				}
			}
		}

		void Text2DRenderer::Draw(int layer)
		{
			if (textRenderData.find(layer) == textRenderData.end())
				return;

			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			shader.Use();
			shader.SetInteger("characterTexture", 0);
			shader.SetMatrix4("projection", Game::mainCamera.GetProjection());

			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(vao);

			for (auto& data : textRenderData[layer])
			{
				for (auto& characterData : data)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, characterData.first);

					for (auto& character : characterData.second)
					{
						auto& color = std::get<1>(character);
						shader.SetVector4f("characterColor", glm::vec4(color[0], color[1], color[2], color[3]));

						auto& vertices = std::get<0>(character);
						glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 2 * 4, vertices.data());
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
					}
				}
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}