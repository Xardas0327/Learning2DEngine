#pragma once

#include <glm/glm.hpp>

namespace Learning2DEngine
{
	namespace Test
	{
		class CompareFloat
		{
		private:
			static const float floatThreshold;
		public:
			static bool IsEqual(const glm::vec2& a, const glm::vec2& b);

			static bool IsEqual(const glm::mat4& a, const glm::mat4& b);
		};
	}
}
