#pragma once

#include "ObjectPoint.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		struct ObjectBox : public ObjectPoint
		{
			glm::vec2 size;
		};
	}
}