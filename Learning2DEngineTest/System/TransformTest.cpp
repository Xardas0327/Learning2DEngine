#include "CppUnitTest.h"

#include <Learning2DEngine/System/GameObjectManager.h>
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
			TEST_METHOD(GetLocalPosition)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject(glm::vec2(1.0f, 2.0f));
				
				Assert::IsTrue(gameObject->transform.GetLocalPosition() == glm::vec2(1.0f, 2.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(SetLocalPosition)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();
				Assert::IsTrue(gameObject->transform.GetLocalPosition() == glm::vec2(0.0f, 0.0f));

				gameObject->transform.SetLocalPosition(glm::vec2(1.0f, 2.0f));
				Assert::IsTrue(gameObject->transform.GetLocalPosition() == glm::vec2(1.0f, 2.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(AddPosition)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject(glm::vec2(1.0f, 2.0f));
				gameObject->transform.AddLocalPosition(glm::vec2(3.0f, 4.0f));

				Assert::IsTrue(gameObject->transform.GetLocalPosition() == glm::vec2(4.0f, 6.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(GetGlobalPosition)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject(glm::vec2(1.0f, 2.0f));
				auto child = manager.CreateGameObject(glm::vec2(5.0f, 6.0f));
				Assert::IsTrue(child->transform.GetGlobalPosition() == glm::vec2(5.0f, 6.0f));

				child->transform.SetParent(parent);
				Assert::IsTrue(child->transform.GetLocalPosition() == glm::vec2(4.0f, 4.0f));
				Assert::IsTrue(child->transform.GetGlobalPosition() == glm::vec2(5.0f, 6.0f));

				child->transform.ClearParent();
				Assert::IsTrue(child->transform.GetLocalPosition() == glm::vec2(5.0f, 6.0f));
				Assert::IsTrue(child->transform.GetGlobalPosition() == glm::vec2(5.0f, 6.0f));

				child->transform.SetParent(parent, false);
				Assert::IsTrue(child->transform.GetGlobalPosition() == glm::vec2(6.0f, 8.0f));

				child->transform.ClearParent(false);
				Assert::IsTrue(child->transform.GetGlobalPosition() == glm::vec2(5.0f, 6.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(SetGlobalPosition)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject(glm::vec2(1.0f, 2.0f));
				auto child = manager.CreateGameObject(glm::vec2(5.0f, 6.0f));
				child->transform.SetParent(parent);
				child->transform.SetGlobalPosition(glm::vec2(10.0f, 10.0f));

				Assert::IsTrue(child->transform.GetLocalPosition() == glm::vec2(9.0f, 8.0f));
				Assert::IsTrue(child->transform.GetGlobalPosition() == glm::vec2(10.0f, 10.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(AddGlobalPosition)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject(glm::vec2(1.0f, 2.0f));
				auto child = manager.CreateGameObject(glm::vec2(5.0f, 6.0f));
				child->transform.SetParent(parent);
				child->transform.AddGlobalPosition(glm::vec2(4.0f, 4.0f));

				Assert::IsTrue(child->transform.GetLocalPosition() == glm::vec2(8.0f, 8.0f));
				Assert::IsTrue(child->transform.GetGlobalPosition() == glm::vec2(9.0f, 10.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(GetScale)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject(glm::vec2(1.0f, 2.0f), glm::vec2(3.0f, 4.0f));

				Assert::IsTrue(gameObject->transform.GetLocalScale() == glm::vec2(3.0f, 4.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(SetScale)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();
				Assert::IsTrue(gameObject->transform.GetLocalScale() == glm::vec2(1.0f, 1.0f));

				gameObject->transform.SetLocalScale(glm::vec2(3.0f, 4.0f));
				Assert::IsTrue(gameObject->transform.GetLocalScale() == glm::vec2(3.0f, 4.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(AddScale)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();
				Assert::IsTrue(gameObject->transform.GetLocalScale() == glm::vec2(1.0f, 1.0f));

				gameObject->transform.AddLocalScale(glm::vec2(3.0f, 4.0f));
				Assert::IsTrue(gameObject->transform.GetLocalScale() == glm::vec2(4.0f, 5.0f));

				gameObject->transform.AddLocalScale(glm::vec2(6.0f, 5.0f));
				Assert::IsTrue(gameObject->transform.GetLocalScale() == glm::vec2(10.0f, 10.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(GetGlobalScale)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f), 0.0f);
				auto child = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(3.0f, 3.0f), 0.0f);
				Assert::IsTrue(child->transform.GetGlobalScale() == glm::vec2(3.0f, 3.0f));

				child->transform.SetParent(parent);
				Assert::IsTrue(child->transform.GetLocalScale() == glm::vec2(1.5f, 1.5f));
				Assert::IsTrue(child->transform.GetGlobalScale() == glm::vec2(3.0f, 3.0f));

				child->transform.ClearParent();
				Assert::IsTrue(child->transform.GetLocalScale() == glm::vec2(3.0f, 3.0f));
				Assert::IsTrue(child->transform.GetGlobalScale() == glm::vec2(3.0f, 3.0f));

				child->transform.SetParent(parent, false);
				Assert::IsTrue(child->transform.GetGlobalScale() == glm::vec2(6.0f, 6.0f));

				child->transform.ClearParent(false);
				Assert::IsTrue(child->transform.GetGlobalScale() == glm::vec2(3.0f, 3.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(SetGlobalScale)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f), 0.0f);
				auto child = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(3.0f, 3.0f), 0.0f);
				child->transform.SetParent(parent);
				child->transform.SetGlobalScale(glm::vec2(10.0f, 10.0f));

				Assert::IsTrue(child->transform.GetLocalScale() == glm::vec2(5.0f, 5.0f));
				Assert::IsTrue(child->transform.GetGlobalScale() == glm::vec2(10.0f, 10.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(AddGlobalScale)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f), 0.0f);
				auto child = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(3.0f, 3.0f), 0.0f);
				child->transform.SetParent(parent);
				child->transform.AddGlobalScale(glm::vec2(2.0f, 2.0f));

				Assert::IsTrue(child->transform.GetLocalScale() == glm::vec2(2.5f, 2.5f));
				Assert::IsTrue(child->transform.GetGlobalScale() == glm::vec2(5.0f, 5.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(GetRotation)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject(glm::vec2(1.0f, 2.0f), glm::vec2(3.0f, 4.0f), 5.0f);

				Assert::IsTrue(gameObject->transform.GetLocalRotation() == 5.0f);

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(SetRotation)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();

				Assert::IsTrue(gameObject->transform.GetLocalRotation() == 0.0f);

				gameObject->transform.SetLocalRotation(15.0f);
				Assert::IsTrue(gameObject->transform.GetLocalRotation() == 15.0f);

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(AddRotation)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();
				Assert::IsTrue(gameObject->transform.GetLocalRotation() == 0.0f);

				gameObject->transform.AddLocalRotation(15.0f);
				Assert::IsTrue(gameObject->transform.GetLocalRotation() == 15.0f);

				gameObject->transform.AddLocalRotation(20.0f);
				Assert::IsTrue(gameObject->transform.GetLocalRotation() == 35.0f);

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(GetGlobalRotation)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 15.0f);
				auto child = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 30.0f);
				Assert::IsTrue(child->transform.GetGlobalRotation() == 30.0f);

				child->transform.SetParent(parent);
				Assert::IsTrue(CompareFloat::IsEqual(child->transform.GetLocalRotation(), 15.0f));
				Assert::IsTrue(CompareFloat::IsEqual(child->transform.GetGlobalRotation(), 30.0f));

				child->transform.ClearParent();
				Assert::IsTrue(CompareFloat::IsEqual(child->transform.GetLocalRotation(), 30.0f));
				Assert::IsTrue(CompareFloat::IsEqual(child->transform.GetGlobalRotation(), 30.0f));

				child->transform.SetParent(parent, false);
				Assert::IsTrue(CompareFloat::IsEqual(child->transform.GetGlobalRotation(), 45.0f));

				child->transform.ClearParent(false);
				Assert::IsTrue(CompareFloat::IsEqual(child->transform.GetGlobalRotation(), 30.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(SetGlobalRotation)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 15.0f);
				auto child = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 30.0f);
				child->transform.SetParent(parent);
				child->transform.SetGlobalRotation(60.0f);

				Assert::IsTrue(CompareFloat::IsEqual(child->transform.GetLocalRotation(), 45.0f));
				Assert::IsTrue(CompareFloat::IsEqual(child->transform.GetGlobalRotation(), 60.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(AddGlobalRotation)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 15.0f);
				auto child = manager.CreateGameObject(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 30.0f);
				child->transform.SetParent(parent);
				child->transform.AddGlobalRotation(60.0f);

				Assert::IsTrue(CompareFloat::IsEqual(child->transform.GetLocalRotation(), 75.0f));
				Assert::IsTrue(CompareFloat::IsEqual(child->transform.GetGlobalRotation(), 90.0f));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(GetModelMatrix)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();
				Assert::IsTrue(gameObject->transform.GetModelMatrix() == glm::mat4(1.0f));

				gameObject->transform.SetLocalPosition(glm::vec2(1.0f, 2.0f));
				gameObject->transform.SetLocalScale(glm::vec2(3.0f, 4.0f));
				gameObject->transform.SetLocalRotation(5.0f);

				glm::mat4 expectedModelMatrix = glm::mat4
				{
					2.9885f, 0.2614f, 0.0f, 0.0f,
					-0.3486f, 3.9847f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					1.18f, 1.876f, 0.0f, 1.0f
				};
				Assert::IsTrue(CompareFloat::IsEqual(gameObject->transform.GetModelMatrix(), expectedModelMatrix));

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(GetParent)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameobject = manager.CreateGameObject();
				Assert::IsTrue(gameobject->transform.GetParent() == nullptr);

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(SetParent)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject();
				auto child = manager.CreateGameObject();
				child->transform.SetParent(parent);
				
				Assert::IsTrue(child->transform.GetParent() == parent);

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(ClearParent)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject();
				auto child = manager.CreateGameObject();
				child->transform.SetParent(parent);

				Assert::IsTrue(child->transform.GetParent() == parent);

				child->transform.ClearParent();
				Assert::IsTrue(child->transform.GetParent() == nullptr);

				manager.DestroyAllGameObjects();
			}
		};

	}
}