#pragma once

#include <glm/glm.hpp>

namespace Learning2DEngine
{
	namespace System
	{
		struct Transform
		{
			glm::vec2 position;
			glm::vec2 size;
			float rotation;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="p">position</param>
			/// <param name="s">size</param>
			/// <param name="r">rotation</param>
			Transform(glm::vec2 p = glm::vec2(0.0f, 0.0f), glm::vec2 s = glm::vec2(1.0f, 1.0f), float r = 0.0f)
				: position(p), size(s), rotation(r)
			{

			}
		};
	}
}

