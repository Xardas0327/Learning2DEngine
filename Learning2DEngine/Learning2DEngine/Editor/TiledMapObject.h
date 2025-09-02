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
			int rows; 
			glm::vec2 size;
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
				float allColumns = static_cast<float>(columns);
				float allRows = static_cast<float>(rows);

				return { 
					column / allColumns, row / allRows,
					(column + 1.0f) / allColumns, row / allRows,
					(column + 1.0f) / allColumns, (row + 1.0f) / allRows,
					column / allColumns, (row + 1.0f) / allRows
				};
			}
		};
	}
}