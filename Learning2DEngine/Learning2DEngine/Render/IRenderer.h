#pragma once

#include "vector"

#include "IRenderData.h"

namespace Learning2DEngine
{
	namespace Render
	{
		struct IRenderer
		{
			virtual void Draw(std::vector<IRenderData*> renderData) = 0;
			virtual ~IRenderer() = default;
		};
	}
}
