#pragma once

#include <map>
#include <string>

#include "Component.h"
#include "GameObject.h"
#include "Property.h"

namespace Learning2DEngine
{
	namespace System
	{
		class PropertyComponent : public virtual Component
		{
			friend class GameObject;
		protected:
			PropertyComponent(GameObject* gameObject)
				: Component(gameObject), properties()
			{

			}

			PropertyComponent(GameObject* gameObject, const std::map<std::string, Property>& properties)
				: Component(gameObject), properties(properties)
			{

			}
		public:
			std::map<std::string, Property> properties;

			virtual ~PropertyComponent() = default;
		};
	}
}
