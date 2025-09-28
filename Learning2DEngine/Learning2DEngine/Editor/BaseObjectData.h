#pragma once

#include <map>
#include <string>

#include "../System/Property.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		struct BaseObjectData
		{
			std::map<std::string, System::Property> properties;

			BaseObjectData(std::map<std::string, System::Property>&& properties)
				: properties(std::move(properties))
			{

			}
		};
	}
}