#include "CppUnitTest.h"

#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/GameObject.h>
#include <Learning2DEngine/System/Component.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Learning2DEngine
{
	namespace System
	{
		TEST_CLASS(GameObjectTest)
		{
			class TestComponent1 : public Component
			{
			public:
				TestComponent1(GameObject* gameObject)
					: Component(gameObject)
				{
				}
			};

			class TestComponent2 : public Component
			{
			public:
				TestComponent2(GameObject* gameObject)
					: Component(gameObject)
				{
				}
			};

		public:
			TEST_METHOD(Create)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();
				Assert::IsNotNull(gameObject);

				manager.DestroyAllGameObjects();
			}
			TEST_METHOD(AddComponent)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();

				auto component = gameObject->AddComponent<TestComponent1>();
				Assert::IsNotNull(component);

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(GetComponent)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();

				gameObject->AddComponent<TestComponent1>();
				gameObject->AddComponent<TestComponent2>();

				auto component1 = gameObject->GetComponent<TestComponent1>();
				Assert::IsNotNull(component1);

				auto component2 = gameObject->GetComponent<TestComponent2>();
				Assert::IsNotNull(component2);

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(GetComponents)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();

				auto component1s = gameObject->GetComponents<TestComponent1>();
				Assert::IsTrue(component1s.empty());

				gameObject->AddComponent<TestComponent1>();
				gameObject->AddComponent<TestComponent1>();
				gameObject->AddComponent<TestComponent2>();

				component1s = gameObject->GetComponents<TestComponent1>();
				Assert::IsTrue(component1s.size() == 2);

				auto component2s = gameObject->GetComponents<TestComponent2>();
				Assert::IsTrue(component2s.size() == 1);

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(Destroy)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();
				gameObject->AddComponent<TestComponent1>();

				gameObject->Destroy();

				auto component1 = gameObject->GetComponent<TestComponent1>();
				Assert::IsNull(component1);

				manager.DestroyAllGameObjects();
			}
		};

	}
}