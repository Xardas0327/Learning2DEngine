#include "CppUnitTest.h"

#include <Learning2DEngine/System/Math.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Learning2DEngine::System;

namespace Learning2DEngine
{
	namespace System
	{
		TEST_CLASS(MathTest)
		{
		public:
			TEST_METHOD(FlipByX)
			{
				auto matrix = glm::mat4x2{
					{1.0f, 2.0f}, {3.0f, 4.0f},
					{5.0f, 6.0f}, {7.0f, 8.0f}
				};

				auto resultMatrix = glm::mat4x2{
					{3.0f, 4.0f}, {1.0f, 2.0f},
					{7.0f, 8.0f}, {5.0f, 6.0f}
				};

				Assert::IsTrue(resultMatrix == Math::FlipByX(matrix));
			}

			TEST_METHOD(FlipByY)
			{
				auto matrix = glm::mat4x2{
					{1.0f, 2.0f}, {3.0f, 4.0f},
					{5.0f, 6.0f}, {7.0f, 8.0f}
				};

				auto resultMatrix = glm::mat4x2{
					{5.0f, 6.0f}, {7.0f, 8.0f},
					{1.0f, 2.0f}, {3.0f, 4.0f}
				};

				Assert::IsTrue(resultMatrix == Math::FlipByY(matrix));
			}
		};

	}
}
