#include "MultiText2DRenderer.h"

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
		MultiText2DRenderer::MultiText2DRenderer()
			: BaseMultiRenderer(), textRenderData()
		{

		}

		void MultiText2DRenderer::InitShader()
		{
			auto& resourceManager = ResourceManager::GetInstance();

			shader = resourceManager.IsShaderExist(ShaderConstant::TEXT2D_SHADER_NAME)
				? &resourceManager.GetShader(ShaderConstant::TEXT2D_SHADER_NAME)
				: &resourceManager.LoadShader(
					ShaderConstant::TEXT2D_SHADER_NAME,
					ShaderConstant::GetText2DVertexShader(),
					ShaderConstant::GetText2DFragmentShader());
		}

		void MultiText2DRenderer::InitVao()
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

		void MultiText2DRenderer::DestroyObject()
		{
			BaseMultiRenderer::DestroyObject();

			textRenderData.clear();
		}

		void MultiText2DRenderer::CalcDynamicDataSize(size_t maxDynamicSize)
		{
			//if the size is not enough or too big, it will be reallocated.
			if (maxDynamicSize > maxObjectSize || maxObjectSize > maxDynamicSize * 2)
			{
				//It allocates 20% more space, so that it does not have to allocate again
				//if there are some dynamic renderers. 
				maxObjectSize = static_cast<size_t>(
					static_cast<float>(maxDynamicSize) * 1.2f
					);

				glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);
				//Multiply by 4, because an object has 4 vertices.
				glBufferData(GL_ARRAY_BUFFER, sizeof(Text2DDynamicData) * maxObjectSize * 4, NULL, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
				//Multiply by 6, because an object (2 triangles) has 6 indices.
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

		void MultiText2DRenderer::SetData(const std::map<int, std::vector<Render::RenderData*>>& renderData)
		{
			GLint maxTextureUnit = RenderManager::GetInstance().GetMaxTextureUnits();
			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			textRenderData.clear();
			size_t maxDynamicSize = 0;
			for (auto& layerData : renderData)
			{
				auto& actualLayerData = textRenderData[layerData.first];

				for (auto& data : layerData.second)
				{
					auto textData = static_cast<Text2DRenderData*>(data);
					if (textData->GetText().size() > 0)
					{
						textCharacterSet.Load(textData->GetFontSizePair());
						CharacterMap& characterMap = textCharacterSet[textData->GetFontSizePair()];

						auto& textMap = textData->GetCharacterVertices();
						for (auto& chPair : textMap)
						{
							const auto& ch = characterMap[chPair.first];
							for (auto& vertices : chPair.second)
							{
								actualLayerData[ch.textureId].emplace_back(
									std::array<float, 32>{
									// vertex 1
									// position						// texture coordinates			// color
									vertices[0][0], vertices[0][1], vertices[0][2], vertices[0][3], textData->color.r, textData->color.g, textData->color.b, textData->color.a,
									// vertex 2
									vertices[1][0], vertices[1][1], vertices[1][2], vertices[1][3], textData->color.r, textData->color.g, textData->color.b, textData->color.a,
									// vertex 3
									vertices[2][0], vertices[2][1], vertices[2][2], vertices[2][3], textData->color.r, textData->color.g, textData->color.b, textData->color.a,
									// vertex 4
									vertices[3][0], vertices[3][1], vertices[3][2], vertices[3][3], textData->color.r, textData->color.g, textData->color.b, textData->color.a
								}
								);
							}
						}
					}
				}

				//Calculate the maximum size of the dynamic data
				int textureUnitNumber = 0;
				size_t actualSize = 0;
				int elementCount = 0;
				for (auto& data : actualLayerData)
				{
					actualSize += data.second.size();
					++textureUnitNumber;
					++elementCount;

					//Check if the texture unit number is arrived to max or this is the last data
					if (textureUnitNumber >= maxTextureUnit || elementCount == actualLayerData.size())
					{
						textureUnitNumber = 0;
						if (actualSize > maxDynamicSize)
							maxDynamicSize = actualSize;
					}
				}
			}

			CalcDynamicDataSize(maxDynamicSize);
		}

		void MultiText2DRenderer::Draw(int layer)
		{
			if (textRenderData.find(layer) == textRenderData.end())
				return;

			GLint maxTextureUnit = RenderManager::GetInstance().GetMaxTextureUnits();
			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			shader->Use();
			shader->SetMatrix4("projection", Game::mainCamera.GetProjection());

			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamic);

			int dynamicSize = 0;
			int objectCount = 0;
			GLint textureUnitId = 0;
			size_t elementCount = 0;
			for (auto& characterData : textRenderData[layer])
			{
				shader->SetInteger(("characterTextures[" + std::to_string(textureUnitId) + "]").c_str(), textureUnitId);
				glActiveTexture(GL_TEXTURE0 + textureUnitId);
				glBindTexture(GL_TEXTURE_2D, characterData.first);

				for (auto& character : characterData.second)
				{
					float tUnitId = static_cast<float>(textureUnitId);;

					std::memcpy(&dynamicData[dynamicSize],
						character.data(),
						sizeof(float) * 8);
					dynamicData[dynamicSize].textureId = tUnitId;
					++dynamicSize;

					std::memcpy(&dynamicData[dynamicSize],
						character.data() + 8,
						sizeof(float) * 8);
					dynamicData[dynamicSize].textureId = tUnitId;
					++dynamicSize;

					std::memcpy(&dynamicData[dynamicSize],
						character.data() + 16,
						sizeof(float) * 8);
					dynamicData[dynamicSize].textureId = tUnitId;
					++dynamicSize;

					std::memcpy(&dynamicData[dynamicSize],
						character.data() + 24,
						sizeof(float) * 8);
					dynamicData[dynamicSize].textureId = tUnitId;
					++dynamicSize;
					++objectCount;
				}
				++textureUnitId;
				++elementCount;

				//Check if the texture unit number is arrived to max or this is the last data
				if (textureUnitId >= maxTextureUnit || elementCount == textRenderData[layer].size())
				{
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Text2DDynamicData) * dynamicSize, dynamicData);
					glDrawElements(GL_TRIANGLES, 6 * objectCount, GL_UNSIGNED_INT, 0);
					dynamicSize = 0;
					objectCount = 0;
					textureUnitId = 0;
				}
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}