#pragma once

#include "map"
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
			virtual void SetData(const std::map<int, std::vector<RenderData*>>& renderData) = 0;
			virtual void Draw(int layer) = 0;
			virtual ~IRenderer() = default;
		};
	}
}
