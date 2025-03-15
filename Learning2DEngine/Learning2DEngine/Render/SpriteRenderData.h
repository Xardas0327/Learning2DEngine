#pragma once

#include <glm/glm.hpp>

#include "RenderData.h"
#include "Texture2D.h"

namespace Learning2DEngine
{
	namespace Render
	{
		struct SpriteRenderData : public RenderData
		{
			Texture2D* texture;
			glm::vec4 color;

			SpriteRenderData(const System::Component* component, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), texture(nullptr)
			{
			}

			SpriteRenderData(const System::Component* component, const Texture2D& texture, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), texture(new Texture2D(texture))
			{
			}

			~SpriteRenderData() override
			{
				if (IsUseTexture())
				{
					delete texture;
				}
			}

			inline bool IsUseTexture() const
			{
				return texture != nullptr;
			}

			inline void ClearTexture()
			{
				texture = nullptr;
			}
		};
	}
}