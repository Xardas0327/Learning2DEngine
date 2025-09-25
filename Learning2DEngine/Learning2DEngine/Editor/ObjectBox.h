#pragma once

#include "ObjectPoint.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		struct ObjectBox : public ObjectPoint
		{
			glm::vec2 size;

			ObjectBox(const glm::vec2& position, const glm::vec2& size, std::map<std::string, System::Property>&& properties)
				: ObjectPoint(position, std::move(properties)), size(size)
			{

			}
		};
	}
}