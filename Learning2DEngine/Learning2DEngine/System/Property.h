#pragma once

#include <string>
#include <glm/glm.hpp>

#include "../DebugTool/Log.h"
#include "PropertyType.h"

namespace Learning2DEngine
{
	namespace System
	{
		class Property final
		{
		private:
			PropertyType type;

			union {
				bool boolValue;
				glm::vec4 colorValue;
				float floatValue;
				int intValue;
			};

			//for text and file
			std::string textValue;
		public:
			Property(bool value)
				: type(PropertyType::Bool), boolValue(value), textValue()
			{
			}

			Property(const glm::vec4& value)
				: type(PropertyType::Color), colorValue(value), textValue()
			{
			}

			Property(float value)
				: type(PropertyType::Float), floatValue(value), textValue()
			{
			}

			Property(int value)
				: type(PropertyType::Int), intValue(value), textValue()
			{
			}

			Property(const std::string& value, PropertyType type)
				: type(type), boolValue(false), textValue(value)
			{
				if (type != PropertyType::String && type != PropertyType::File)
				{
					L2DE_LOG_ERROR("TiledProperty: the type is not valid for string constructor.");
				}
			}

			Property(std::string&& value, PropertyType type)
				: type(type), boolValue(false), textValue(std::move(value))
			{
				if (type != PropertyType::String && type != PropertyType::File)
				{
					L2DE_LOG_ERROR("TiledProperty: the type is not valid for string constructor.");
				}
			}

			~Property() = default;

			inline PropertyType GetType() const
			{
				return type;
			}

			inline bool GetBool() const
			{
				return boolValue;
			}

			inline glm::vec4 GetColor() const
			{
				return colorValue;
			}

			inline float GetFloat() const
			{
				return floatValue;
			}

			inline int GetInt() const
			{
				return intValue;
			}

			inline const std::string& GetString() const
			{
				return textValue;
			}

			inline const std::string& GetFile() const
			{
				return textValue;
			}
		};
	}
}