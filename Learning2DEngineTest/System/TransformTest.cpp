#include "CppUnitTest.h"

#include <Learning2DEngine/System/Transform.h>
#include "../Test/CompareFloat.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
	using namespace Test;

	namespace System
	{
		TEST_CLASS(TransformTest)
		{
		public:
			TEST_METHOD(GetPosition)
			{
				Transform transform(glm::vec2(1.0f, 2.0f));

				Assert::IsTrue(transform.GetPosition() == glm::vec2(1.0f, 2.0f));
			}

			TEST_METHOD(SetPosition)
			{
				Transform transform;
				Assert::IsTrue(transform.GetPosition() == glm::vec2(0.0f, 0.0f));

				transform.SetPosition(glm::vec2(1.0f, 2.0f));
				Assert::IsTrue(transform.GetPosition() == glm::vec2(1.0f, 2.0f));
			}

			TEST_METHOD(AddPosition)
			{
				Transform transform(glm::vec2(1.0f, 2.0f));
				transform.AddPosition(glm::vec2(3.0f, 4.0f));
				Assert::IsTrue(transform.GetPosition() == glm::vec2(4.0f, 6.0f));
			}

			TEST_METHOD(GetScale)
			{
				Transform transform(glm::vec2(1.0f, 2.0f), glm::vec2(3.0f, 4.0f));

				Assert::IsTrue(transform.GetScale() == glm::vec2(3.0f, 4.0f));
			}

			TEST_METHOD(SetScale)
			{
				Transform transform;
				Assert::IsTrue(transform.GetScale() == glm::vec2(1.0f, 1.0f));

				transform.SetScale(glm::vec2(3.0f, 4.0f));
				Assert::IsTrue(transform.GetScale() == glm::vec2(3.0f, 4.0f));
			}

			TEST_METHOD(AddScale)
			{
				Transform transform;
				Assert::IsTrue(transform.GetScale() == glm::vec2(1.0f, 1.0f));

				transform.AddScale(glm::vec2(3.0f, 4.0f));
				Assert::IsTrue(transform.GetScale() == glm::vec2(4.0f, 5.0f));

				transform.AddScale(glm::vec2(6.0f, 5.0f));
				Assert::IsTrue(transform.GetScale() == glm::vec2(10.0f, 10.0f));
			}

			TEST_METHOD(GetRotation)
			{
				Transform transform(glm::vec2(1.0f, 2.0f), glm::vec2(3.0f, 4.0f), 5.0f);

				Assert::IsTrue(transform.GetRotation() == 5.0f);
			}

			TEST_METHOD(SetRotation)
			{
				Transform transform;
				Assert::IsTrue(transform.GetRotation() == 0.0f);

				transform.SetRotation(15.0f);
				Assert::IsTrue(transform.GetRotation() == 15.0f);
			}

			TEST_METHOD(AddRotation)
			{
				Transform transform;
				Assert::IsTrue(transform.GetRotation() == 0.0f);

				transform.AddRotation(15.0f);
				Assert::IsTrue(transform.GetRotation() == 15.0f);

				transform.AddRotation(20.0f);
				Assert::IsTrue(transform.GetRotation() == 35.0f);
			}

			TEST_METHOD(GetModelMatrix)
			{
				Transform transform;
				Assert::IsTrue(transform.GetModelMatrix() == glm::mat4(1.0f));

				transform.SetPosition(glm::vec2(1.0f, 2.0f));
				transform.SetScale(glm::vec2(3.0f, 4.0f));
				transform.SetRotation(5.0f);

				const Transform transform2 = transform;
				glm::mat4 expectedModelMatrix = glm::mat4
				{
					2.9885f, 0.2614f, 0.0f, 0.0f,
					-0.3486f, 3.9847f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					1.18f, 1.876f, 0.0f, 1.0f
				};
				Assert::IsTrue(CompareFloat::IsEqual(transform2.GetModelMatrix(),expectedModelMatrix));
			}
		};

	}
}