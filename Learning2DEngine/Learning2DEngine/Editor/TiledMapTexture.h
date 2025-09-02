#pragma once

#include <glm/glm.hpp>


#include "../Render/Texture2D.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		struct TiledMapTexture final
		{
			int firstGid;
			int columns;
			int rows; 
			glm::vec2 objectSize;
			Render::Texture2D* texture;

			bool HasNumber(int gid) const
			{
				return gid >= firstGid && gid < firstGid + (columns * rows);
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

				return { 
					column / (float)columns, row / (float)rows,
					(column + 1.0f) / (float)columns, row / (float)rows,
					(column + 1.0f) / (float)columns, (row + 1.0f) / (float)rows,
					column / (float)columns, (row + 1.0f) / (float)rows
				};
			}
		};
	}
}