#pragma once

#include <glm/glm.hpp>

#include "RenderData.h"
#include "Texture2D.h"
#include "Texture2DContainer.h"

namespace Learning2DEngine
{
	namespace Render
	{
#define L2DE_SPRITE_UV_DEFAULT glm::mat4x2 { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f}

		class SpriteRenderData : public RenderData, public Texture2DContainer
		{
		public:
			glm::vec4 color;
			bool isUseCameraView;

			/// <summary>
			/// The texture coordinate order:
			/// Top Left,
			/// Top Right,
			/// Bottom Right,
			/// Bottom Left
			/// </summary>
			glm::mat4x2 uvMatrix;

			SpriteRenderData(const System::Component* component, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), isUseCameraView(true),
				uvMatrix(L2DE_SPRITE_UV_DEFAULT), Texture2DContainer()
			{
			}

			SpriteRenderData(const System::Component* component, bool isUseCameraView, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), isUseCameraView(isUseCameraView),
				uvMatrix(L2DE_SPRITE_UV_DEFAULT), Texture2DContainer()
			{
			}

			SpriteRenderData(const System::Component* component, const Texture2D& texture, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), isUseCameraView(true),
				uvMatrix(L2DE_SPRITE_UV_DEFAULT), Texture2DContainer(texture)
			{
			}

			SpriteRenderData(const System::Component* component, const Texture2D& texture, bool isUseCameraView, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), isUseCameraView(isUseCameraView),
				uvMatrix(L2DE_SPRITE_UV_DEFAULT), Texture2DContainer(texture)
			{
			}
		};
	}
}