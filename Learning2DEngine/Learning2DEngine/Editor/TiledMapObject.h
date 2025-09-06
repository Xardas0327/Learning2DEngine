#pragma once

#include <glm/glm.hpp>


#include "../Render/Texture2D.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		struct TiledMapObject final
		{
			int firstGid;
			int columns;
			int tileCount;
			int spacing;
			int margin;
			glm::vec2 tiledSize;
			glm::vec2 imageSize;
			Render::Texture2D* texture;

			TiledMapObject()
				: firstGid(0), columns(0), tileCount(0), spacing(0), margin(0),
				tiledSize(0.0f), imageSize(0.0f), texture(nullptr)
			{
			}

			~TiledMapObject() = default;

			bool HasNumber(int gid) const
			{
				return gid >= firstGid && gid < firstGid + tileCount;
			}

			/// <summary>
			/// The texture coordinate order:
			/// Top Left,
			/// Top Right,
			/// Bottom Right,
			/// Bottom Left
			/// </summary>
			glm::mat4x2 GetUV(int gid) const
			{
				int actualId = gid - firstGid;
				float column = static_cast<float>(actualId % columns);
				float row = static_cast<float>(actualId / columns);

				glm::vec2 topLeft(
					static_cast<float>(margin) + column * (tiledSize.x + spacing),
					static_cast<float>(margin) + row * (tiledSize.y + spacing)
				);

				//if it is not start from 0, add 1 pixel offset to avoid texture bleeding
				float startX = (topLeft.x + static_cast<float>(column != 0.0f)) / imageSize.x;
				float startY = (topLeft.y + static_cast<float>(row != 0.0f)) / imageSize.y;

				float endX = (topLeft.x + tiledSize.x) / imageSize.x;
				float endY = (topLeft.y + tiledSize.y) / imageSize.y;

				return {
					startX, startY,
					endX, startY,
					endX, endY,
					startX,endY
				};
			}
		};
	}
}