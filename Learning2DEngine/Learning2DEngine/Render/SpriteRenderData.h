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

			SpriteRenderData(System::GameObject* gameObject, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(gameObject), color(color), texture(nullptr)
			{
			}

			SpriteRenderData(System::GameObject* gameObject, const Texture2D& texture, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(gameObject), color(color), texture(new Texture2D(texture))
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