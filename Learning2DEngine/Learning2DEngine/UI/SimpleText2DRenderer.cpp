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
			shader->SetMatrix4("cameraView", Game::mainCamera.GetViewMatrix());

			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(vao);

			for (auto& data : textRenderData[layer])
			{
				auto textData = static_cast<Text2DRenderData*>(data);

				shader->SetVector4f("characterColor", textData->color);
				shader->SetInteger("useCamera", textData->isUseCameraView ? GL_TRUE : GL_FALSE);

				textCharacterSet.Load(textData->GetFontSizePair());
				CharacterMap& characterMap = textCharacterSet[textData->GetFontSizePair()];

				auto& textMap = textData->GetCharacterVertices();
				for (auto& chPair : textMap)
				{
					const auto& ch = characterMap[chPair.first];
					for (auto& vertices : chPair.second)
					{
						glBindTexture(GL_TEXTURE_2D, ch.textureId);

						glBindBuffer(GL_ARRAY_BUFFER, vbo);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &vertices[0][0]);
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