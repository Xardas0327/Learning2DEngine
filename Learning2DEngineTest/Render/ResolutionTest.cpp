#include "CppUnitTest.h"

#include <Learning2DEngine/Render/Resolution.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
	namespace Render
	{
		TEST_CLASS(ResolutionTest)
		{
		public:
			TEST_METHOD(GetWidth)
			{
				Resolution resolution(800, 600);
				Assert::IsTrue(resolution.GetWidth() == 800);
			}

			TEST_METHOD(GetHeight)
			{
				Resolution resolution(800, 600);
				Assert::IsTrue(resolution.GetHeight() == 600);
			}

			TEST_METHOD(ToString)
			{
				Resolution resolution(800, 600);
				std::string expected = "800x600";
				Assert::IsTrue(resolution.ToString() == expected);
			}

			TEST_METHOD(ToVec2)
			{
				Resolution resolution(800, 600);
				Assert::IsTrue(resolution.ToVec2() == glm::vec2(800.0f, 600.0f));
			}
		};

	}
}