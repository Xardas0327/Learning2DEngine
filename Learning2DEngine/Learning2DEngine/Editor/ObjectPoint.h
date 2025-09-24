#pragma once

#include <glm/glm.hpp>
#include <map>

#include "IObjectData.h"
#include "../System/Property.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		struct ObjectPoint : public IObjectData
		{
			glm::vec2 position;
			std::map<std::string, System::Property> properties;
		};
	}
}