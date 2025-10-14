#include "CompareFloat.h"

namespace Learning2DEngine
{
	namespace Test
	{
		const float CompareFloat::floatThreshold = 0.001f;

		bool CompareFloat::IsEqual(const glm::vec2& a, const glm::vec2& b)
		{
			glm::vec2 difference = a - b;
			return glm::abs(difference.x) < floatThreshold && glm::abs(difference.y) < floatThreshold;
		}

		bool CompareFloat::IsEqual(const glm::vec4& a, const glm::vec4& b)
		{
			glm::vec4 difference = a - b;
			return glm::abs(difference.x) < floatThreshold && glm::abs(difference.y) < floatThreshold
				&& glm::abs(difference.z) < floatThreshold && glm::abs(difference.a) < floatThreshold;
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

		bool CompareFloat::IsEqual(const glm::mat4x2& a, const glm::mat4x2& b)
		{
			bool isFine = true;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 2; ++j)
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
}