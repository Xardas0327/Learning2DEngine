#include "CompareFloat.h"

namespace TestHelper
{
	const float CompareFloat::floatThreshold = 0.001f;

	bool CompareFloat::IsEqual(const glm::vec2& a, const glm::vec2& b)
	{
		glm::vec2 difference = a - b;
		return glm::abs(difference.x) < floatThreshold && glm::abs(difference.y) < floatThreshold;
	}

	bool CompareFloat::IsEqual(const glm::mat4& a, const glm::mat4& b)
	{
		bool isFine = true;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (glm::abs(a[i][j] - b[i][j]) > floatThreshold)
				{
					isFine = false;
					break;
				}
			}

			if (!isFine)
				break;
		}

		return isFine;
	}
}