#include "CppUnitTest.h"

#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/ComponentManager.h>
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
				ComponentManager::GetInstance().Clear();
			}

			TEST_METHOD(AddComponent)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject = manager.CreateGameObject();

				auto component = gameObject->AddComponent<TestComponent1>();
				Assert::IsNotNull(component);

				manager.DestroyAllGameObjects();
				ComponentManager::GetInstance().Clear();
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
				ComponentManager::GetInstance().Clear();
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
				ComponentManager::GetInstance().Clear();
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
				ComponentManager::GetInstance().Clear();
			}

			TEST_METHOD(IsActive)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto gameObject1 = manager.CreateGameObject();
				Assert::IsTrue(gameObject1->IsActive());

				auto gameObject2 = manager.CreateGameObject(false);
				Assert::IsFalse(gameObject2->IsActive());

				manager.DestroyAllGameObjects();
			}

			TEST_METHOD(SetActive)
			{
				auto& manager = GameObjectManager::GetInstance();

				auto parent = manager.CreateGameObject();
				auto child = manager.CreateGameObject();
				child->transform.SetParent(parent);

				// Both of them are active.
				Assert::IsTrue(parent->IsActive());
				Assert::IsTrue(child->IsActive());

				parent->SetActive(true);
				child->SetActive(false);
				Assert::IsTrue(parent->IsActive());
				Assert::IsFalse(child->IsActive());

				parent->SetActive(false);
				child->SetActive(false);
				Assert::IsFalse(parent->IsActive());
				Assert::IsFalse(child->IsActive());

				parent->SetActive(false);
				child->SetActive(true);
				Assert::IsFalse(parent->IsActive());
				Assert::IsFalse(child->IsActive());

				manager.DestroyAllGameObjects();
			}
		};

	}
}