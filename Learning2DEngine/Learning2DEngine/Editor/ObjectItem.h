#pragma once

#include "ObjectType.h"
#include "BaseObjectData.h"
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
			BaseObjectData* data;
		public:
			const ObjectType type;

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

			ObjectItem(const ObjectItem& other)
				: type(other.type), data(nullptr)
			{
				switch (type)
				{
				case ObjectType::Point:
					data = new ObjectPoint(*static_cast<ObjectPoint*>(other.data));
					break;
				case ObjectType::Box:
					data = new ObjectBox(*static_cast<ObjectBox*>(other.data));
					break;
				case ObjectType::Ellipse:
					data = new ObjectEllipse(*static_cast<ObjectEllipse*>(other.data));
					break;
				case ObjectType::Image:
					data = new ObjectImage(*static_cast<ObjectImage*>(other.data));
					break;
				}
			}

			~ObjectItem()
			{
				delete data;
			}

			const BaseObjectData* GetData() const
			{
				return data;
			}
		};
	}
}