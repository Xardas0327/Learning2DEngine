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
			: shader(), vao(0), ebo(0), vboDynamic(0), maxObjectSize(0),
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
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, NULL, GL_DYNAMIC_DRAW);


			glGenBuffers(1, &vboDynamic);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Text2DDynamicData), NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Text2DDynamicData), (void*)offsetof(Text2DDynamicData, position));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Text2DDynamicData), (void*)offsetof(Text2DDynamicData, textCoord));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Text2DDynamicData), (void*)offsetof(Text2DDynamicData, color));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Text2DDynamicData), (void*)offsetof(Text2DDynamicData, textureId));


			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			maxObjectSize = 1;
			dynamicData = new Text2DDynamicData[maxObjectSize * 4];
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
			int maxDynamicSize = 0;
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

							//Calculate next character position
							startPosition += rotationMatrix *
								glm::vec2(
									// bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
									(ch.advance >> 6) * textData->component->gameObject->transform.scale.x,
									0);
						}
					}
				}

				for (auto& data : actualLayerData)
				{
					int actualSize = 0;
					for (auto& map : data)
					{
						actualSize += map.second.size();
					}
					if (actualSize > maxDynamicSize)
						maxDynamicSize = actualSize;
				}
			}

			//if the size is not enough or too big, it will be reallocated.
			if (maxDynamicSize > maxObjectSize || maxObjectSize > maxDynamicSize * 2)
			{
				//It allocates 20% more space, so that it does not have to allocate again
				//if there are some dynamic renderers. 
				maxObjectSize = static_cast<float>(maxDynamicSize) * 1.2f;

				glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
				glBufferData(GL_ARRAY_BUFFER, sizeof(Text2DDynamicData) * maxObjectSize * 4, NULL, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * maxObjectSize * 6, NULL, GL_DYNAMIC_DRAW);

				delete[] dynamicData;
				dynamicData = new Text2DDynamicData[maxObjectSize * 4];
				unsigned int* dynamicIndices = new unsigned int[maxObjectSize * 6];

				for (int i = 0; i < maxObjectSize; i++)
				{
					dynamicIndices[i * 6 + 0] = i * 4 + 0;
					dynamicIndices[i * 6 + 1] = i * 4 + 1;
					dynamicIndices[i * 6 + 2] = i * 4 + 3;
					dynamicIndices[i * 6 + 3] = i * 4 + 1;
					dynamicIndices[i * 6 + 4] = i * 4 + 2;
					dynamicIndices[i * 6 + 5] = i * 4 + 3;
				}
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(float) * 6 * maxObjectSize, dynamicIndices);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				delete[] dynamicIndices;
			}

		}

		void Text2DRenderer::Draw(int layer)
		{
			if (textRenderData.find(layer) == textRenderData.end())
				return;

			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			shader.Use();
			shader.SetMatrix4("projection", Game::mainCamera.GetProjection());

			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);

			for (auto& data : textRenderData[layer])
			{
				int dynamicSize = 0;
				int objectCount = 0;
				int textureUnitId = 0;
				for (auto& characterData : data)
				{
					shader.SetInteger(("characterTextures["+std::to_string(textureUnitId) + "]").c_str(), textureUnitId);
					glActiveTexture(GL_TEXTURE0 + textureUnitId);
					glBindTexture(GL_TEXTURE_2D, characterData.first);

					for (auto& character : characterData.second)
					{
						dynamicData[dynamicSize].position[0] = std::get<0>(character)[0];
						dynamicData[dynamicSize].position[1] = std::get<0>(character)[1];
						dynamicData[dynamicSize].textCoord[0] = 1.0f;
						dynamicData[dynamicSize].textCoord[1] = 1.0f;
						std::memcpy(dynamicData[dynamicSize].color, std::get<1>(character).data(), sizeof(dynamicData[dynamicSize].color));
						dynamicData[dynamicSize].textureId = textureUnitId;
						++dynamicSize;

						dynamicData[dynamicSize].position[0] = std::get<0>(character)[2];
						dynamicData[dynamicSize].position[1] = std::get<0>(character)[3];
						dynamicData[dynamicSize].textCoord[0] = 1.0f;
						dynamicData[dynamicSize].textCoord[1] = 0.0f;
						std::memcpy(dynamicData[dynamicSize].color, std::get<1>(character).data(), sizeof(dynamicData[dynamicSize].color));
						dynamicData[dynamicSize].textureId = textureUnitId;
						++dynamicSize;

						dynamicData[dynamicSize].position[0] = std::get<0>(character)[4];
						dynamicData[dynamicSize].position[1] = std::get<0>(character)[5];
						dynamicData[dynamicSize].textCoord[0] = 0.0f;
						dynamicData[dynamicSize].textCoord[1] = 0.0f;
						std::memcpy(dynamicData[dynamicSize].color, std::get<1>(character).data(), sizeof(dynamicData[dynamicSize].color));
						dynamicData[dynamicSize].textureId = textureUnitId;
						++dynamicSize;

						dynamicData[dynamicSize].position[0] = std::get<0>(character)[6];
						dynamicData[dynamicSize].position[1] = std::get<0>(character)[7];
						dynamicData[dynamicSize].textCoord[0] = 0.0f;
						dynamicData[dynamicSize].textCoord[1] = 1.0f;
						std::memcpy(dynamicData[dynamicSize].color, std::get<1>(character).data(), sizeof(dynamicData[dynamicSize].color));
						dynamicData[dynamicSize].textureId = textureUnitId;
						++dynamicSize;
						++objectCount;
					}
					++textureUnitId;
				}
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Text2DDynamicData)* dynamicSize, dynamicData);
				glDrawElements(GL_TRIANGLES, 6 * objectCount, GL_UNSIGNED_INT, 0);
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}