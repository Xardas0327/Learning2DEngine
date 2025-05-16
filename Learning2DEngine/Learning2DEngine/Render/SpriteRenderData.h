#pragma once

#include <glm/glm.hpp>

#include "RenderData.h"
#include "Texture2D.h"
#include "Texture2DContainer.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class SpriteRenderData : public RenderData, public Texture2DContainer
		{
		public:
			glm::vec4 color;

			SpriteRenderData(const System::Component* component, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), Texture2DContainer()
			{
			}

			SpriteRenderData(const System::Component* component, const Texture2D& texture, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), Texture2DContainer(texture)
			{
			}
		};
	}
}