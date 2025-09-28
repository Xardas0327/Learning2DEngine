#pragma once

#include <glm/glm.hpp>
#include <map>
#include <string>

#include "BaseObjectData.h"
#include "../System/Property.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		struct ObjectPoint : public BaseObjectData
		{
			glm::vec2 position;

			ObjectPoint(const glm::vec2& position, std::map<std::string, System::Property>&& properties)
				: BaseObjectData(std::move(properties)), position(position)
			{

			}
		};
	}
}