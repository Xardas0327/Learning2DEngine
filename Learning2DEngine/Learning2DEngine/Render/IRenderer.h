#pragma once

#include "vector"

#include "RenderData.h"

namespace Learning2DEngine
{
	namespace Render
	{
		struct IRenderer
		{
			virtual void Init() = 0;
			virtual void Destroy() = 0;
			virtual void Draw(std::vector<RenderData*> renderData) = 0;
			virtual ~IRenderer() = default;
		};
	}
}
