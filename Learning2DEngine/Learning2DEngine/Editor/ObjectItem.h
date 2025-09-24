#pragma once

#include "ObjectType.h"
#include "ObjectPoint.h"
#include "ObjectBox.h"
#include "ObjectEllipse.h"
#include "ObjectImage.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		class ObjectItem
		{
		public:
			const ObjectType type;
			const IObjectData* const data;

			ObjectItem(ObjectPoint&& point)
				: type(ObjectType::Point), data(new ObjectPoint(std::move(point)))
			{
			}

			ObjectItem(ObjectBox&& box)
				: type(ObjectType::Box), data(new ObjectBox(std::move(box)))
			{
			}

			ObjectItem(ObjectEllipse&& ellipse)
				: type(ObjectType::Ellipse), data(new ObjectEllipse(std::move(ellipse)))
			{
			}

			ObjectItem(ObjectImage&& image)
				: type(ObjectType::Image), data(new ObjectImage(std::move(image)))
			{
			}

			~ObjectItem()
			{
				delete data;
			}
		};
	}
}