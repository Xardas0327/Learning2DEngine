#include "CppUnitTest.h"

#include <Learning2DEngine/System/Random.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
	namespace System
	{
		TEST_CLASS(RandomTest)
		{
		public:
			TEST_METHOD(GetNumberInt)
			{
				int number = Random::GetNumber(1, 2);
				Assert::IsTrue(number >= 1 && number < 2);
			}

			TEST_METHOD(GetNumberIntWithException)
			{
				bool exceptionThrown = false;
				try
				{
					int number = Random::GetNumber(1, 1);
				}
				catch (const std::exception&)
				{
					exceptionThrown = true;
				}
				Assert::IsTrue(exceptionThrown);
			}

			TEST_METHOD(GetNumberFloat)
			{
				float number = Random::GetNumber(1.0f, 2.0f);
				Assert::IsTrue(number >= 1.0f && number <= 2.0f);
			}

			TEST_METHOD(GetNumberFloatWithException)
			{
				bool exceptionThrown = false;
				try
				{
					float number = Random::GetNumber(1.0f, 1.0f);
				}
				catch (const std::exception&)
				{
					exceptionThrown = true;
				}
				Assert::IsTrue(exceptionThrown);
			}
		};

	}
}