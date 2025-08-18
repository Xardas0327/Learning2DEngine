#include "CppUnitTest.h"

#include <Learning2DEngine/System/Camera.h>
#include "../Test/CompareFloat.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Learning2DEngine::Render;
using namespace Learning2DEngine::Test;

namespace Learning2DEngine
{
	namespace System
	{
		TEST_CLASS(CameraTest)
		{
		public:
			TEST_METHOD(GetPosition)
			{
				Camera camera1;
				Assert::IsTrue(camera1.GetPosition() == glm::vec2(0.0f, 0.0f));

				Camera camera2(glm::vec2(10.0f, 20.0f));
				Assert::IsTrue(camera2.GetPosition() == glm::vec2(10.0f, 20.0f));
			}

			TEST_METHOD(SetPosition)
			{
				Camera camera;
				camera.SetPosition(glm::vec2(1.0f, 2.0f));
				Assert::IsTrue(camera.GetPosition() == glm::vec2(1.0f, 2.0f));

				camera.SetPosition(glm::vec2(3.0f, 1.0f));
				Assert::IsTrue(camera.GetPosition() == glm::vec2(3.0f, 1.0f));
			}

			TEST_METHOD(GetRotation)
			{
				Camera camera1;
				Assert::IsTrue(camera1.GetRotation() == 0.0f);

				Camera camera2(glm::vec2(0.0f, 0.0f), 10.0f);
				Assert::IsTrue(camera2.GetRotation() == 10.0f);
			}

			TEST_METHOD(SetRotation)
			{
				Camera camera;
				camera.SetRotation(45.0f);
				Assert::IsTrue(camera.GetRotation() == 45.0f);

				camera.SetRotation(15.0f);
				Assert::IsTrue(camera.GetRotation() == 15.0f);
			}

			TEST_METHOD(MoveWithoutIsWorldMoving)
			{
				Camera camera;

				camera.Move(glm::vec2(1.0f, 0.0f), false);
				Assert::IsTrue(camera.GetPosition() == glm::vec2(1.0f, 0.0f));

				camera.Move(glm::vec2(2.0f, 0.0f), false);
				Assert::IsTrue(camera.GetPosition() == glm::vec2(3.0f, 0.0f));
			}

			TEST_METHOD(MoveWithIsWorldMoving)
			{
				Camera camera;

				camera.Move(glm::vec2(1.0f, 0.0f), true);
				Assert::IsTrue(camera.GetPosition() == glm::vec2(1.0f, 0.0f));

				camera.Move(glm::vec2(2.0f, 0.0f), true);
				Assert::IsTrue(camera.GetPosition() == glm::vec2(3.0f, 0.0f));
			}

			TEST_METHOD(MoveWithIsWorldMovingWithAngle)
			{
				Camera camera;
				camera.SetRotation(90.0f);

				camera.Move(glm::vec2(1.0f, 0.0f), true);
				Assert::IsTrue(camera.GetPosition() == glm::vec2(1.0f, 0.0f));

				camera.Move(glm::vec2(2.0f, 0.0f), true);
				Assert::IsTrue(camera.GetPosition() == glm::vec2(3.0f, 0.0f));
			}

			TEST_METHOD(MoveWithoutIsWorldMovingWithAngle)
			{
				Camera camera;
				camera.SetRotation(90.0f);

				camera.Move(glm::vec2(1.0f, 0.0f), false);
				Assert::IsTrue(CompareFloat::IsEqual(camera.GetPosition(), glm::vec2(0.0f, 1.0f)));

				camera.Move(glm::vec2(2.0f, 0.0f), false);
				Assert::IsTrue(CompareFloat::IsEqual(camera.GetPosition(), glm::vec2(0.0f, 3.0f)));
			}

			TEST_METHOD(Rotate)
			{
				Camera camera;
				camera.Rotate(15.0f);
				Assert::IsTrue(camera.GetRotation() == 15.0f);

				camera.Rotate(45.0f);
				Assert::IsTrue(camera.GetRotation() == 60.0f);
			}

			TEST_METHOD(SetPositionRotation)
			{
				Camera camera;
				camera.SetPositionRotation(glm::vec2(1.0f, 2.0f), 15.0f);
				Assert::IsTrue(camera.GetPosition() == glm::vec2(1.0f, 2.0f));
				Assert::IsTrue(camera.GetRotation() == 15.0f);

				camera.SetPositionRotation(glm::vec2(3.0f, 1.0f), 45.0f);
				Assert::IsTrue(camera.GetPosition() == glm::vec2(3.0f, 1.0f));
				Assert::IsTrue(camera.GetRotation() == 45.0f);
			}

			TEST_METHOD(GetViewMatrix)
			{
				Camera camera;
				Assert::IsTrue(camera.GetViewMatrix() == glm::mat4(1.0f));

				camera.SetPositionRotation(glm::vec2(1.0f, 2.0f), 90.0f);
				glm::mat4 expectedViewMatrix = glm::mat4
				{
					 0.0f, -1.0f, 0.0f, 0.0f,
					 1.0f,  0.0f, 0.0f, 0.0f,
					 0.0f,  0.0f, 1.0f, 0.0f,
					-2.0f, 1.0f, 0.0f , 1.0f
				};
				Assert::IsTrue(CompareFloat::IsEqual(camera.GetViewMatrix(), expectedViewMatrix));
			}

			TEST_METHOD(GetProjection)
			{
				Camera camera;
				Assert::IsTrue(camera.GetProjection() == glm::mat4(0.0f));

				camera.SetResolution(Resolution(800, 600));
				glm::mat4 expectedProjection = glm::mat4
				{
					0.0025f, 0.0f, 0.0f, 0.0f,
					0.0f, -0.003f, 0.0f, 0.0f,
					0.0f, 0.0f, -1.0f, 0.0f,
					-1.0f, 1.0f, 0.0f, 1.0f
				};
				Assert::IsTrue(CompareFloat::IsEqual(camera.GetProjection(), expectedProjection));
			}

			TEST_METHOD(GetResolution)
			{
				Camera camera;
				Assert::IsTrue(camera.GetResolution().ToVec2() == Resolution(0, 0).ToVec2());
			}

			TEST_METHOD(SetResolution)
			{
				Camera camera;
				camera.SetResolution(Resolution(800, 600));
				Assert::IsTrue(camera.GetResolution().ToVec2() == Resolution(800, 600).ToVec2());
			}
		};

	}
}