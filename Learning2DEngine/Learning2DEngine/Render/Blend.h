#pragma once

#include <utility>

namespace Learning2DEngine
{
	namespace Render
	{
		// First is the source factor.
		// Second is the destination factor.
		typedef std::pair<unsigned int, unsigned int> BlendFuncFactor;
	}
}