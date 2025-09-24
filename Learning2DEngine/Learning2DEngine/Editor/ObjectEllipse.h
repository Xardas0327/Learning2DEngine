#pragma once

#include "ObjectPoint.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		//The size.x will be used only, because of CircleColliderComponent
		struct ObjectEllipse : public ObjectPoint
		{
			glm::vec2 size;
		};
	}
}